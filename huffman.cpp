#include "huffman.h"

void Huffman::create_pq()
{
	int i;

	create_node_array();

	for (i = 0; i < size; ++i)
		pq.push(node_array[i]);
}

void Huffman::create_node_array()
{
	int i, count;
	int freq[MAX_SIZE] = { 0 }; // frequence of each char
	char in_char;

	// read from file
	while (!in_file.eof())
	{
		in_file.get(in_char);
		if (in_file.eof())
			break;
		freq[(unsigned char)in_char]++;
	}

	count = 0;
	for (i = 0; i < MAX_SIZE; ++i)
	{
		if (freq[i] <= 0)
			continue;
		Node_ptr node = new Huffman_node();
		node->id = i;
		node->freq = freq[i];
		node->code = "";
		node->left = nullptr;
		node->right = nullptr;
		node->parent = nullptr;

		node_array[count++] = node;
	}
	// insert pseudo-EOF , frequence is 1
	Node_ptr node = new Huffman_node();
	node->id = PSEUDO_EOF;
	node->freq = 1;
	node->code = "";
	node->left = nullptr;
	node->right = nullptr;
	node->parent = nullptr;

	node_array[count++] = node;

	size = count;
}

void Huffman::create_huffman_tree()
{
	root = nullptr;

	while (!pq.empty())
	{
		Node_ptr first = pq.top();
		pq.pop();
		if (pq.empty())
		{
			root = first;
			break;
		}
		Node_ptr second = pq.top();
		pq.pop();
		Node_ptr new_node = new Huffman_node();
		new_node->freq = first->freq + second->freq;

		if (first->freq <= second->freq)
		{
			new_node->left = first;
			new_node->right = second;
		}
		else
		{
			new_node->left = second;
			new_node->right = first;
		}
		first->parent = new_node;
		second->parent = new_node;

		pq.push(new_node);
	}
}

void Huffman::calculate_huffman_codes()
{
	if (root == nullptr)
	{
		cout << "Build the huffman tree failed or no characters are counted." << endl;
		return;
	}

	if (root->left != nullptr)
		create_map_table(root->left, true);
	if (root->right != nullptr)
		create_map_table(root->right, false);
}

void Huffman::create_map_table(const Node_ptr node, bool left)
{
	if (left)
		node->code = node->parent->code + "0";
	else
		node->code = node->parent->code + "1";

	// 如果是叶子节点，则是一个“有效”节点，加入编码表
	if (node->left == nullptr && node->right == nullptr)
		table[node->id] = node->code;
	else
	{
		if (node->left != nullptr)
			create_map_table(node->left, true);
		if (node->right != nullptr)
			create_map_table(node->right, false);
	}
}

void Huffman::do_compress()
{
	char in_char;
	unsigned char out_char;
	string code, out_string;
	map<int, string>::iterator table_it;

	// number of nodes(include pseudo-EOF) + huffman tree + re-code text

	// number of nodes
	out_file << size << endl;

	// 2nd to (size+1)th line  write huffman tree，e.g. "43 00100"
	for (table_it = table.begin(); table_it != table.end(); ++table_it)
	{
		out_file << table_it->first << " " << table_it->second << endl;
	}

	
	// write text
	in_file.clear();
	in_file.seekg(ios::beg);
	code.clear();
	while (!in_file.eof())
	{
		in_file.get(in_char);
		if (in_file.eof())
			break;
		// find its huffman code from map table
		table_it = table.find((unsigned char)in_char);
		if (table_it != table.end())
			code += table_it->second;
		else
		{
			cout << "Can't find the huffman code of character" << in_char << endl;
			return;
		}
		
		while (code.length() >= 8)
		{
			out_char = 0;
			
			for (int i = 0; i < 8; i++)
			{
				if (code[i] == '1')
					out_char += 1 << (7 - i);
			}
			out_file << out_char;
			code = code.substr(8, code.length() - 8);
		}
	}

	// insert pseudo-EOF
	table_it = table.find(PSEUDO_EOF);
	if (table_it != table.end())
		code += table_it->second;
	else
	{
		cout << "Can't find the huffman code of pseudo-EOF." << endl;
		return ;
	}
	while (code.length() >= 8)
	{
		out_char = 0;
		for (int i = 0; i < 8; i++)
		{
			if (code[i] == '1')
				out_char += 1 << (7 - i);
		}
		out_file << out_char;
		code = code.substr(8, code.length() - 8);
	}
	if (code.length())
	{
		out_char = 0;
		for (auto i = 0; i < code.length(); i++)
		{
			if (code[i] == '1')
				out_char += (1 << (code.length() - 1 - i));  
		}
		out_char <<= (8 - code.length());  // add 0 to make a whole byte
		out_file << out_char;
	}

	in_file.close();
	out_file.close();
}

void Huffman::rebuid_huffman_tree()
{
	int i, j, id, length;
	string code;
	Node_ptr node, tmp, new_node;

	root = new Huffman_node();
	root->left = nullptr;
	root->right = nullptr;
	root->parent = nullptr; 

	in_file >> size;
	if (size > MAX_SIZE)
	{
		cout << "The number of nodes is not valid, maybe the compressed file has been broken." << endl;
		return ;
	}

	for (i = 0; i < size; ++i)
	{
		in_file >> id;
		in_file >> code;

		length = code.length();
		node = root;
		for (j = 0; j < length; ++j)
		{
			if ('0' == code[j])
				tmp = node->left;
			else if ('1' == code[j])
				tmp = node->right;
			else
			{
				cout << "Decode error, huffman code is not made up with 0 or 1." << endl;
				return;
			}

			if (tmp == nullptr)
			{
				new_node = new Huffman_node();
				new_node->left = nullptr;
				new_node->right = nullptr;
				new_node->parent = node;

				if (j == length - 1)
				{
					new_node->id = id;
					new_node->code = code;
				}

				if ('0' == code[j])
					node->left = new_node;
				else
					node->right = new_node;

				tmp = new_node;
			}
			else if (j == length - 1)
			{
				cout << "Huffman code is not valid, maybe the compressed file has been broken." << endl;
				return ;
			}
			else if (tmp->left == nullptr && tmp->right == nullptr)
			{
				cout << "Huffman code is not valid, maybe the compressed file has been broken." << endl;
				return;
			}
			node = tmp;
		}

	}
}

void Huffman::do_decompress()
{
	bool pseudo_eof;
	int i, id;
	char in_char;
	string out_string;
	unsigned char u_char, flag;
	Node_ptr node;

	out_string.clear();
	node = root;
	pseudo_eof = false;

	in_file.get();           // to ignore the \n !!!!!!!!!!!!!!!!!!
	

	while (in_file.get(in_char))
	{
		
		u_char = (unsigned char)in_char;
		flag = 0x80;
		for (i = 0; i < 8; ++i)
		{

			if (u_char & flag)
				node = node->right;
			else
				node = node->left;

			if (node->left == nullptr && node->right == nullptr)
			{
				id = node->id;
				if (id == PSEUDO_EOF)
				{
					pseudo_eof = true;
					break;
				}
				else
				{
					// int to char is safe
					out_file  << (char)node->id;
					node = root;
				}
			}
			flag = flag >> 1;
		}
		if (pseudo_eof)
			break;
	}

	in_file.close();
	out_file.close();
}

Huffman::Huffman(string in_file_name, string out_file_name)
{
	in_file.open(in_file_name, ios::in|ios::binary);
	if (!in_file)
	{
		cout << "Open file error" << endl;
		return;
	}

	out_file.open(out_file_name, ios::out|ios::binary);
	if (!out_file)
	{
		cout << "Open file error" << endl;
		return;
	}
}

Huffman::~Huffman()
{
	in_file.close();
	out_file.close();
}

void Huffman::compress()
{
	create_pq();
	create_huffman_tree();
	calculate_huffman_codes();
	do_compress();
}

void Huffman::decompress()
{
	rebuid_huffman_tree();
	do_decompress();
}