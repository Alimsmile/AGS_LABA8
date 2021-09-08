#define _CRT_SECURE_NO_WARNINGS
#include "CMesh.h"

CMesh::CMesh(void)
{
	VertexCount = 0;
	VBO_Index = 0;
	VAO_Index = 0;
}

int CMesh::LoadOBJ(const char* filename)
{
	// вектор для хранения информации о полигонах
	std::vector<Face> faces;
	std::vector <glm::vec3> v;
	std::vector <glm::vec2> vt;
	std::vector <glm::vec3> vn;

	FILE * file = fopen(filename, "r");
	if (file == NULL) {
		printf("Не удалось открыть файл !\n");
		return 1;
	}
	while (1) {
		char lineHeader[128];
		// читаем первый символ в файле 
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = Конец файла. Заканчиваем цикл чтения 
				   // else : парсим строку 
		if (strcmp(lineHeader, "#") == 0) {

		}
		if (strcmp(lineHeader, "v") == 0) {
			//чтение одного комплекта геом координат вершины 
			float x, y, z;
			fscanf(file, "%f %f %f\n", &x, &y, &z);
		//	printf("%f %f %f \n", x, y, z);
			//помещение геометрических координат в контейнер 
			v.push_back(glm::vec3(x, y, z));
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			float s, t;
			fscanf(file, "%f %f\n", &s, &t);
		//	printf("%f %f \n", s, t);
			vt.push_back(glm::vec2(s, t));
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			float b, n, m;
			fscanf(file, "%f %f %f\n", &b, &n, &m);
		//	printf("%f %f %f \n", b, n, m);
			vn.push_back(glm::vec3(b, n, m));
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			Face face;
			for (int i = 0; i < 3; i++) 
			{
				fscanf(file, "%i/%i/%i", &face.v[i], &face.vt[i], &face.vn[i]);
			//	printf("%i   %i   %i   \n", face.v[i], face.vt[i], face.vn[i]);
				
			}
			//cout << "****************************" << endl;
			faces.push_back(face);
			VertexCount++;

		}

	}
	VertexCount= VertexCount*3;
	Vertex *verteces = new Vertex[VertexCount];
	//cout << "Путь к файлу " << namefile << endl;
	//cout << "Количество вершин VertexCount " << VertexCount << endl;
	//cout << "Количество вершин faces.size() " << faces.size()<< endl;
	for (int i = 0; i < faces.size() ; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			verteces[i * 3 + j].vPosition[0] = v[faces[i].v[j] - 1].x;
			verteces[i * 3 + j].vPosition[1] = v[faces[i].v[j] - 1].y;
			verteces[i * 3 + j].vPosition[2] = v[faces[i].v[j] - 1].z;
		//	cout << verteces[i * 3 + j].vPosition[0] << "\t"<<verteces[i * 3 + j].vPosition[1] << "\t" << verteces[i * 3 + j].vPosition[2] << endl;

			verteces[i * 3 + j].vTexCoord[0] = vt[faces[i].vt[j] - 1].x;
			verteces[i * 3 + j].vTexCoord[1] = vt[faces[i].vt[j] - 1].y;

			verteces[i * 3 + j].vNormal[0] = vn[faces[i].vn[j] - 1].x;
			verteces[i * 3 + j].vNormal[1] = vn[faces[i].vn[j] - 1].y;
			verteces[i * 3 + j].vNormal[2] = vn[faces[i].vn[j] - 1].z;
		}
	}

	if (VAO_Index != 0)
	{
		glDeleteVertexArrays(1, &VAO_Index);
		VAO_Index = 0;
	}
	if (VBO_Index != 0)
	{
		glDeleteBuffers(1, &VBO_Index);
		VBO_Index = 0;
	}
	//Привязываем к VBO буферу
	 glGenBuffers(1,&VBO_Index);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
	 glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * VertexCount, verteces, GL_STATIC_DRAW);
	//Привязываем к VAO буферу
	 glGenVertexArrays(1, &VAO_Index);
	 glBindVertexArray(VAO_Index);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);

	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(offsetof(Vertex, vPosition)));
	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(offsetof(Vertex, vNormal)));
	 glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(offsetof(Vertex, vTexCoord)));
	 //вкл атрибуты
	 glEnableVertexAttribArray(0);
	 glEnableVertexAttribArray(1);
	 glEnableVertexAttribArray(2);
	 glBindVertexArray(0);

	return 0;
}

void CMesh::Render(void)
{
	// начинаем работу с созданным VAO
	glBindVertexArray(VAO_Index);
	// рисуем вершины
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);
	// отвязываем VAO чтобы не испортить
	glBindVertexArray(0);
}

void CMesh::LoadRect()
{
	GLfloat vertices[12]={
		-1,  1,
		-1, -1,
		1,  1,
		1,  1,
		-1, -1,
		1, -1
	};

	glGenBuffers(1, &VBO_Index);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, &vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO_Index);
	glBindVertexArray(VAO_Index);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	VertexCount = 6;
}