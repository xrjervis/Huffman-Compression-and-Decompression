This program is implemented by C++ 11.

Source codes can be seen on my [Github](https://github.com/xrjervis/Huffman-Compression-and-Decompression)

#### Features:
- Use priority queue to store nodes. Then I just need the pq.top() to get the minimum value node in the node sequence and new node value can be placed in the appropriate position of the sequence.

- Use STL map to store each pair of huffman code.

- Use pseudo-eof (id = 256, frequence = 1) and insert it at the end of the encoded text. Then the program will know where the text ends.

- Write the frequence table in compressed.txt, and then when decompressing I can rebuild the huffman table from this table.

##### main.cpp
	The entrance of the program. 
	To control the process of console.

##### huffman.cpp
	Implement each function declared in huffman.h

##### huffman.h
	Include the declaration of each function.

#### Screenshots
![](http://7xtifo.com1.z0.glb.clouddn.com/HM_PIC_1.png)

![](http://7xtifo.com1.z0.glb.clouddn.com/HM_PIC_2.png)

![](http://7xtifo.com1.z0.glb.clouddn.com/HM_PIC_3.png)

![](http://7xtifo.com1.z0.glb.clouddn.com/HM_PIC_4.png)

![](http://7xtifo.com1.z0.glb.clouddn.com/HM_PIC_5.png)



