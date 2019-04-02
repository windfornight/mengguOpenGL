思路：
	核心是，vbo和ibo的数据形式。
	对于obj模型文件，这部分的数据存储在f（面)的行中，其中，一个顶点数据（position, texcoord, nornal）的索引是ibo数据， 具体的顶点
数据又能构成了vbo中的一个数据（重复数据的处理），注意，这些数据的依赖数据存在由于v， vt， vn数据组中

用到的知识  stringstream, vector, string