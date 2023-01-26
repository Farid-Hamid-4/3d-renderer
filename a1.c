#define PY_SSIZE_T_CLEAN

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "a1.h"
#include <Python/Python.h>
#include <Python/unicodeobject.h>

/**HELPER FUNCTIONS**/
char *getLine(FILE *fp)
{
	char buffer[100000] = "";
	char c;
	int i = 0;
	while ((c = getc(fp)) != '\n')
	{
		buffer[i] = c;
		i++;
	}
	char *line = malloc(strlen(buffer) + 1 * sizeof(char));
	strcpy(line, buffer);
	line[strlen(buffer)] = '\0';
	return line;
}

char **tokenize(char *string, char *delims)
{
	int rowCount = 0;
	int memory = 1;
	char **tokens = malloc(memory * sizeof(char *));

	char buffer[1000000] = "";
	strcpy(buffer, string);
	char *token = strtok(buffer, delims);

	while (token != NULL)
	{
		tokens[rowCount] = malloc(strlen(token) + 1);
		strcpy(tokens[rowCount], token);
		rowCount++;
		memory++;
		tokens = realloc(tokens, memory * sizeof(char *));
		token = strtok(NULL, delims);
	}

	tokens[rowCount] = malloc(1 * sizeof(char));
	strcpy(tokens[rowCount], "\0");
	return tokens;
}

void printStrings(char **strings)
{
	int i = 0;
	for (i = 0; strcmp(strings[i], "\0") != 0; i++)
	{
		printf("%s\n", strings[i]);
	}
}

void freeStrings(char **strings)
{
	int i = 0;
	for (i = 0; strcmp(strings[i], "\0") != 0; i++)
	{
		free(strings[i]);
	}
	free(strings[i]);
	free(strings);
}

/**CONSTRUCTORS**/

FileContent *driver(char *fileName)
{
	if (fileName == NULL)
	{
		fprintf(stderr, "Invalid file name\n");
		return NULL;
	}
	FILE *fp = fopen(fileName, "r");
	if (!fp)
	{
		fprintf(stderr, "Could not open file\n");
		return NULL;
	}
	FileContent *fc = makeFileContent();
	int validFile = parser(fc, fp);
	if (validFile == 0)
	{
		fprintf(stderr, "File content is invalid\n");
		deleteFileData(fc);
		fc = NULL;
	}
	fclose(fp);
	return fc;
}

FileContent *makeFileContent(void)
{
	FileContent *fc = malloc(1 * sizeof(FileContent));
	fc->normals = NULL;
	fc->vertices = NULL;
	fc->textures = NULL;
	fc->faces = NULL;
	fc->numFaces = 0;
	fc->numNorms = 0;
	fc->numTexts = 0;
	fc->numVerts = 0;
	return fc;
}

Face *makeFace(char *string)
{
	Face *f = malloc(1 * sizeof(Face));
	char **tokens = tokenize(string, "\t\n\r ");
	int i;
	f->numElems = 0;
	for (i = 0; strcmp(tokens[i], "\0") != 0; i++)
	{
	}

	f->numElems = i;
	f->firstElem = parseFaceElement(tokens[0]);
	f->secondElem = parseFaceElement(tokens[1]);
	f->thirdElem = parseFaceElement(tokens[2]);
	if (f->numElems == 4)
	{
		f->fourthElem = parseFaceElement(tokens[3]);
	}
	freeStrings(tokens);
	return f;
}

FaceElement *makeFaceElement(int vId, int tId, int nId)
{
	FaceElement *fe = malloc(1 * sizeof(FaceElement));
	fe->vertexId = vId;
	fe->textureId = tId;
	fe->normalId = nId;
	return fe;
}

FaceElement *parseFaceElement(char *string)
{
	int count = 0;
	unsigned int i;
	int flag = 0;
	int vId = -1;
	int tId = -1;
	int nId = -1;
	for (i = 0; i < strlen(string); i++)
	{
		if (string[i] == '/')
		{
			count++;
			if (string[i + 1] == '/')
				flag = 1;
		}
	}
	if (count == 0)
	{
		vId = atoi(string);

		/*vertex only*/
	}
	else if (count == 1)
	{
		char **tokens = tokenize(string, "/");
		vId = atoi(tokens[0]);
		tId = atoi(tokens[1]);
		freeStrings(tokens);
		/*vertex and texture*/
	}
	else if (count == 2)
	{
		if (flag == 1)
		{
			/*vertex and normal*/
			char **tokens = tokenize(string, "/");
			vId = atoi(tokens[0]);
			nId = atoi(tokens[1]);
			freeStrings(tokens);
		}
		else
		{
			/*all three*/
			char **tokens = tokenize(string, "/");
			vId = atoi(tokens[0]);
			tId = atoi(tokens[1]);
			nId = atoi(tokens[2]);

			freeStrings(tokens);
		}
	}
	FaceElement *fe = makeFaceElement(vId, tId, nId);
	return fe;
}

int parser(FileContent *fc, FILE *fp)
{
	char c = getc(fp);

	if (c == EOF)
		return 1;
	if (c == '#' || c == 'g' || c == 'o')
	{
		char *discard = getLine(fp);
		free(discard);
		return parser(fc, fp);
	}
	else if (c == 'v')
	{
		/*get vertex, texture or normal*/
		char *line = getLine(fp);
		if (line[0] == 't')
		{
			/*texture*/
			if (validTextureString(line) == 0)
			{
				free(line);
				return 0;
			}
			char **tokens = tokenize(line, "\t\n\r t");
			free(line);
			addTexture(fc, atof(tokens[0]), atof(tokens[1]));
			freeStrings(tokens);
		}
		else if (line[0] == 'n')
		{
			if (validNormalString(line) == 0)
			{
				free(line);
				return 0;
			}
			/*normal*/
			char **tokens = tokenize(line, "\t\n\r n");
			free(line);

			addNormal(fc, atof(tokens[0]), atof(tokens[1]), atof(tokens[2]));
			freeStrings(tokens);
		}
		else if (line[0] == '\t' || line[0] == ' ')
		{
			/*vertex*/
			if (validVertexString(line) == 0)
			{
				free(line);
				return 0;
			}
			char **tokens = tokenize(line, "\t\n\r ");
			free(line);
			addVertex(fc, atof(tokens[0]), atof(tokens[1]), atof(tokens[2]));
			freeStrings(tokens);
		}
		else
		{
			/*error*/
			free(line);
			return 0;
		}
	}
	else if (c == 'f')
	{
		/**get face**/
		char *line = getLine(fp);
		if (validFaceString(line) == 0)
		{
			free(line);
			return 0;
		}

		addFace(fc, line);
		free(line);
	}
	return parser(fc, fp);
}

void addTexture(FileContent *fc, float x, float y)
{
	if (fc->numTexts == 0)
	{
		fc->textures = malloc(1 * sizeof(float *));
		fc->textures[0] = malloc(2 * sizeof(float));
		fc->textures[0][0] = x;
		fc->textures[0][1] = y;
	}
	else
	{
		int i = fc->numTexts;
		fc->textures = realloc(fc->textures, (i + 1) * sizeof(float *));
		fc->textures[i] = malloc(2 * sizeof(float));
		fc->textures[i][0] = x;
		fc->textures[i][1] = y;
	}
	fc->numTexts++;
}

void addNormal(FileContent *fc, float x, float y, float z)
{
	if (fc->numNorms == 0)
	{
		fc->normals = malloc(1 * sizeof(float *));
		fc->normals[0] = malloc(3 * sizeof(float));
		fc->normals[0][0] = x;
		fc->normals[0][1] = y;
		fc->normals[0][2] = z;
	}
	else
	{
		int i = fc->numNorms;
		fc->normals = realloc(fc->normals, (i + 1) * sizeof(float *));
		fc->normals[i] = malloc(3 * sizeof(float));
		fc->normals[i][0] = x;
		fc->normals[i][1] = y;
		fc->normals[i][2] = z;
	}
	fc->numNorms++;
}

void addVertex(FileContent *fc, float x, float y, float z)
{
	if (fc->numVerts == 0)
	{
		fc->vertices = malloc(1 * sizeof(float *));
		fc->vertices[0] = malloc(3 * sizeof(float));
		fc->vertices[0][0] = x;
		fc->vertices[0][1] = y;
		fc->vertices[0][2] = z;
	}
	else
	{
		int i = fc->numVerts;
		fc->vertices = realloc(fc->vertices, (i + 1) * sizeof(float *));
		fc->vertices[i] = malloc(3 * sizeof(float));
		fc->vertices[i][0] = x;
		fc->vertices[i][1] = y;
		fc->vertices[i][2] = z;
	}
	fc->numVerts++;
}

void addFace(FileContent *fc, char *face)
{
	if (fc->numFaces == 0)
	{
		fc->faces = malloc(1 * sizeof(Face *));
		fc->faces[0] = makeFace(face);
	}
	else
	{
		fc->faces = realloc(fc->faces, (fc->numFaces + 1) * sizeof(Face *));
		fc->faces[fc->numFaces] = makeFace(face);
	}
	fc->numFaces++;
}

/**memory clean up**/

void deleteFaceElement(void *d)
{
	if (d == NULL)
		return;
	free(d);
}
void deleteFace(void *d)
{
	if (d == NULL)
		return;
	Face *f = (Face *)d;
	deleteFaceElement(f->firstElem);

	deleteFaceElement(f->secondElem);

	deleteFaceElement(f->thirdElem);
	if (f->numElems == 4)
		deleteFaceElement(f->fourthElem);
	free(f);
}
void deleteFileData(void *d)
{
	if (d == NULL)
		return;
	FileContent *fc = d;
	int i = 0;
	for (i = 0; i < fc->numVerts; i++)
		free(fc->vertices[i]);
	for (i = 0; i < fc->numNorms; i++)
		free(fc->normals[i]);
	for (i = 0; i < fc->numTexts; i++)
		free(fc->textures[i]);
	for (i = 0; i < fc->numFaces; i++)
	{
		deleteFace(fc->faces[i]);
	}
	if (fc->vertices)
		free(fc->vertices);
	if (fc->normals)
		free(fc->normals);
	if (fc->textures)
		free(fc->textures);
	if (fc->faces)
		free(fc->faces);
	free(fc);
}

/**printers**/

char *printFace(FileContent *fc, Face *f, int n)
{
	int numElems = f->numElems;
	char *s = malloc(10000 * sizeof(char));
	strcpy(s, "");
	if (fc == NULL || f == NULL)
		return s;
	char buffer[1000] = "";
	int idx1, idx2, idx3, idx4;
	idx1 = idx2 = idx3 = idx4 = -1;
	float *v1 = NULL;
	float *v2 = NULL;
	float *v3 = NULL;
	float *v4 = NULL;

	if (numElems == 3)
	{
		idx1 = f->firstElem->vertexId - 1;
		idx2 = f->secondElem->vertexId - 1;
		idx3 = f->thirdElem->vertexId - 1;
		v1 = fc->vertices[idx1];
		v2 = fc->vertices[idx2];
		v3 = fc->vertices[idx3];

		sprintf(buffer, "f%d v\t\t%f %f %f\t%f %f %f\t%f %f %f", n, v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2]);
		strcat(s, buffer);
		strcat(s, "\n\n");
		strcpy(buffer, "");
		if (f->firstElem->textureId != -1)
		{
			idx1 = f->firstElem->textureId - 1;
			idx2 = f->secondElem->textureId - 1;
			idx3 = f->thirdElem->textureId - 1;
			v1 = fc->textures[idx1];
			v2 = fc->textures[idx2];
			v3 = fc->textures[idx3];
			sprintf(buffer, "f%d vt\t%f %f\t\t%f %f\t\t%f %f", n, v1[0], v1[1], v2[0], v2[1], v3[0], v3[1]);
			strcat(s, buffer);
			strcat(s, "\n\n");
			strcpy(buffer, "");
		}
		if (f->firstElem->normalId != -1)
		{
			idx1 = f->firstElem->normalId - 1;
			idx2 = f->secondElem->normalId - 1;
			idx3 = f->thirdElem->normalId - 1;
			v1 = fc->normals[idx1];
			v2 = fc->normals[idx2];
			v3 = fc->normals[idx3];
			sprintf(buffer, "f%d vn\t\t%f %f %f\t%f %f %f\t%f %f %f", n, v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2]);
			strcat(s, buffer);
			strcat(s, "\n\n");
			strcpy(buffer, "");
		}
	}
	else
	{
		idx1 = f->firstElem->vertexId - 1;
		idx2 = f->secondElem->vertexId - 1;
		idx3 = f->thirdElem->vertexId - 1;
		idx4 = f->fourthElem->vertexId - 1;
		v1 = fc->vertices[idx1];
		v2 = fc->vertices[idx2];
		v3 = fc->vertices[idx3];
		v4 = fc->vertices[idx4];

		sprintf(buffer, "f%d v\t\t%f %f %f\t%f %f %f\t%f %f %f\t%f %f %f", n, v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2], v4[0], v4[1], v4[2]);
		strcat(s, buffer);
		strcat(s, "\n\n");
		strcpy(buffer, "");
		if (f->firstElem->textureId != -1)
		{
			idx1 = f->firstElem->textureId - 1;
			idx2 = f->secondElem->textureId - 1;
			idx3 = f->thirdElem->textureId - 1;
			idx4 = f->fourthElem->textureId - 1;
			v1 = fc->textures[idx1];
			v2 = fc->textures[idx2];
			v3 = fc->textures[idx3];
			v4 = fc->textures[idx4];
			sprintf(buffer, "f%d vt\t\t%f %f\t\t%f %f\t\t%f %f\t\t%f %f", n, v1[0], v1[1], v2[0], v2[1], v3[0], v3[1], v4[0], v4[1]);
			strcat(s, buffer);
			strcat(s, "\n\n");
			strcpy(buffer, "");
		}
		if (f->firstElem->normalId != -1)
		{
			idx1 = f->firstElem->normalId - 1;
			idx2 = f->secondElem->normalId - 1;
			idx3 = f->thirdElem->normalId - 1;
			idx4 = f->fourthElem->normalId - 1;
			v1 = fc->normals[idx1];
			v2 = fc->normals[idx2];
			v3 = fc->normals[idx3];
			v4 = fc->normals[idx4];
			sprintf(buffer, "f%d vn\t\t%f %f %f\t%f %f %f\t%f %f %f\t%f %f %f", n, v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2], v4[0], v4[1], v4[2]);
			strcat(s, buffer);
			strcat(s, "\n\n");
			strcpy(buffer, "");
		}
	}
	return s;
}

char *printFileData(FileContent *fc)
{
	char *s = malloc(100 * sizeof(char));
	strcpy(s, "\0");
	if (fc == NULL)
		return s;
	int memSize = 100;
	int i;
	for (i = 0; i < fc->numFaces; i++)
	{
		char *temp = printFace(fc, fc->faces[i], i + 1);
		memSize += strlen(temp) + 2;
		s = realloc(s, memSize * sizeof(char));
		strcat(s, temp);
		free(temp);
	}
	return s;
}

/**validators**/

int validVertexString(char *s)
{
	char **tokens = tokenize(s, " \r\n\t");
	int i;
	for (i = 0; strcmp(tokens[i], "\0") != 0; i++)
	{
	}
	freeStrings(tokens);
	if (i != 3)
	{
		fprintf(stderr, "Invalid Vertex: %s\n", s);

		return 0;
	}

	return 1;
}
int validNormalString(char *s)
{
	char **tokens = tokenize(s, "n \r\n\t");
	int i;
	for (i = 0; strcmp(tokens[i], "\0") != 0; i++)
	{
	}

	freeStrings(tokens);
	if (i != 3)
	{
		fprintf(stderr, "Invalid Normal: %s", s);
		return 0;
	}

	return 1;
}

int validTextureString(char *s)
{

	char **tokens = tokenize(s, "t \r\n\t");
	int i;
	for (i = 0; strcmp(tokens[i], "\0") != 0; i++)
	{
	}

	freeStrings(tokens);
	if (i != 2)
	{
		fprintf(stderr, "Invalid Texture: %s\n", s);

		return 0;
	}

	return 1;
}

int validFaceString(char *s)
{
	char **tokens = tokenize(s, " \t\r\n");
	int i;
	int flag = 1;
	for (i = 0; strcmp(tokens[i], "\0") != 0; i++)
	{
	}
	if (i != 3 && i != 4)
	{
		fprintf(stderr, "Invalid Face: %s\n", s);

		freeStrings(tokens);
		return 0;
	}

	for (i = 0; strcmp(tokens[i], "\0") != 0; i++)
	{
		flag = validateElemString(tokens[i]);
		if (flag == 0)
			break;
	}
	if (flag == 0)
		fprintf(stderr, "Invalid Face: %s\n", s);

	freeStrings(tokens);
	return flag;
}
int validateElemString(char *s)
{
	int len = strlen(s) - 1;
	if (s[0] == '/' || s[len] == '/')
		return 0;
	int count = 0;
	unsigned int i = 0;
	for (i = 0; i < strlen(s); i++)
		if (s[i] == '/')
			count++;
	if (count != 0 && count != 1 && count != 2)
		return 0;
	return 1;
}

/*****A2 FUNCTIONS****/

PPM *newPPM(PyObject *pValue)
{
	PPM *ppm = malloc(1 * sizeof(PPM));
	const char *code = PyUnicode_AS_DATA(PyList_GetItem(pValue, 0));
	ppm->code[0] = 'P';
	strcat(ppm->code, code);

	ppm->width = PyLong_AsLong(PyList_GetItem(pValue, 1));
	ppm->height = PyLong_AsLong(PyList_GetItem(pValue, 2));
	ppm->depth = PyLong_AsLong(PyList_GetItem(pValue, 3));
	int rLen, gLen, bLen = 0;
	long **rgbs = getRGBvalues(pValue, &rLen, &gLen, &bLen);
	ppm->numReds = rLen;
	ppm->numGreens = gLen;
	ppm->numBlues = bLen;
	ppm->reds = rgbs[0];
	ppm->greens = rgbs[1];
	ppm->blues = rgbs[2];
	free(rgbs);

	return ppm;
}

void printPPM(PPM *ppm)
{
	printf("%s\n", ppm->code);
	printf("%ld %ld\n", ppm->width, ppm->height);
	printf("%ld\n", ppm->depth);
	long w = ppm->width;
	int k = 1;
	int i = 0;
	int n = ppm->numBlues;
	for (i = 0; i < n; i++)
	{
		if (k % w == 0)
		{
			printf("%ld %ld %ld\n", ppm->reds[i], ppm->greens[i], ppm->blues[i]);
		}
		else
		{
			printf("%ld %ld %ld     ", ppm->reds[i], ppm->greens[i], ppm->blues[i]);
		}
		k++;
	}
	printf("\n");
}

void freePPM(PPM *ppm)
{
	free(ppm->reds);
	free(ppm->greens);
	free(ppm->blues);
	free(ppm);
}

long **getRGBvalues(PyObject *list, int *rLen, int *gLen, int *bLen)
{
	long **rgbs = malloc(3 * sizeof(long *));
	int rIdx = 0;
	int gIdx = 0;
	int bIdx = 0;
	PyObject *rValue;
	PyObject *gValue;
	PyObject *bValue;
	Py_ssize_t i, n;
	n = PyObject_Length(list);
	rgbs[0] = malloc((n / 3) * sizeof(long) + 5);
	rgbs[1] = malloc((n / 3) * sizeof(long) + 5);
	rgbs[2] = malloc((n / 3) * sizeof(long) + 5);
	for (i = 4; i < n; i += 3)
	{
		rValue = PyList_GetItem(list, i);
		gValue = PyList_GetItem(list, i + 1);
		bValue = PyList_GetItem(list, i + 2);

		long rNum = PyLong_AsLong(rValue);
		long gNum = PyLong_AsLong(gValue);
		long bNum = PyLong_AsLong(bValue);
		rgbs[0][rIdx] = rNum;
		rgbs[1][gIdx] = gNum;
		rgbs[2][bIdx] = bNum;
		rIdx++;
		gIdx++;
		bIdx++;
	}

	*rLen = rIdx;
	*gLen = gIdx;
	*bLen = bIdx;
	return rgbs;
}

/**A3 functions**/
void makeJavascript(FileContent *fc, PPM *ppm)
{
	FILE *jsFile = fopen("loaddata.js", "w");
	if (!jsFile)
	{
		fprintf(stderr, "Could not open file 'lodadata.js'\n");
		exit(0);
	}

	// call the one for .obj
	verticicesToJs(fc, jsFile);
	normalsToJs(fc, jsFile);
	texturesToJs(fc, jsFile);
	indexToJs(fc, jsFile);
	getdistance(jsFile);
	getVertexCount(jsFile, fc);
	heightToJs(ppm, jsFile);
	widthToJs(ppm, jsFile);
	rgbToJs(ppm, jsFile);

	fclose(jsFile);
}

void verticicesToJs(FileContent *fd, FILE *fileName)
{

	fprintf(fileName, "function loadvertices() {\nreturn [\n");
	int nf = fd->numFaces;
	for (int i = 0; i < nf; i++)
	{
		Face *f = fd->faces[i];
		vertexToJs(fd, f, fileName);
	}
	fprintf(fileName, "];\n\n}\n\n");
}
void vertexToJs(FileContent *fd, Face *f, FILE *fileName)
{
	FaceElement *e1 = f->firstElem;
	FaceElement *e2 = f->secondElem;
	FaceElement *e3 = f->thirdElem;
	FaceElement *e4 = NULL;
	if (f->numElems == 4)
		e4 = f->fourthElem;

	int v1 = e1->vertexId;
	int v2 = e2->vertexId;
	int v3 = e3->vertexId;
	int v4 = -1;
	if (e4 != NULL)
		v4 = e4->vertexId;
	float x, y, z;
	x = y = z = -1.0;
	x = fd->vertices[v1 - 1][0];
	y = fd->vertices[v1 - 1][1];
	z = fd->vertices[v1 - 1][2];
	fprintf(fileName, "%f,%f,%f,\n", x, y, z);

	x = fd->vertices[v2 - 1][0];
	y = fd->vertices[v2 - 1][1];
	z = fd->vertices[v2 - 1][2];
	fprintf(fileName, "%f,%f,%f,\n", x, y, z);

	x = fd->vertices[v3 - 1][0];
	y = fd->vertices[v3 - 1][1];
	z = fd->vertices[v3 - 1][2];
	fprintf(fileName, "%f,%f,%f,\n", x, y, z);

	if (v4 != -1)
	{
		x = fd->vertices[v4 - 1][0];
		y = fd->vertices[v4 - 1][1];
		z = fd->vertices[v4 - 1][2];
		fprintf(fileName, "%f,%f,%f,\n", x, y, z);
	}
}

void normalsToJs(FileContent *fd, FILE *fileName)
{
	fprintf(fileName, "function loadnormals() {\nreturn [\n");
	int nf = fd->numFaces;
	if (fd->numNorms > 0)
		for (int i = 0; i < nf; i++)
		{
			Face *f = fd->faces[i];
			normalToJs(fd, f, fileName);
		}
	fprintf(fileName, "];\n\n}\n\n");
}
void normalToJs(FileContent *fd, Face *f, FILE *fileName)
{
	FaceElement *e1 = f->firstElem;
	FaceElement *e2 = f->secondElem;
	FaceElement *e3 = f->thirdElem;
	FaceElement *e4 = NULL;
	if (f->numElems == 4)
		e4 = f->fourthElem;

	int v1 = e1->normalId;
	int v2 = e2->normalId;
	int v3 = e3->normalId;
	int v4 = -1;
	if (e4 != NULL)
		v4 = e4->normalId;
	float x, y, z;
	x = y = z = -1.0;

	x = fd->normals[v1 - 1][0];
	y = fd->normals[v1 - 1][1];
	z = fd->normals[v1 - 1][2];
	//(*occupied)[v1 - 1] = v1;
	fprintf(fileName, "%f,%f,%f,\n", x, y, z);

	x = fd->normals[v2 - 1][0];
	y = fd->normals[v2 - 1][1];
	z = fd->normals[v2 - 1][2];
	//(*occupied)[v2 - 1] = v1;
	fprintf(fileName, "%f,%f,%f,\n", x, y, z);

	x = fd->normals[v3 - 1][0];
	y = fd->normals[v3 - 1][1];
	z = fd->normals[v3 - 1][2];
	fprintf(fileName, "%f,%f,%f,\n", x, y, z);

	if (v4 != -1)
	{
		x = fd->normals[v4 - 1][0];
		y = fd->normals[v4 - 1][1];
		z = fd->normals[v4 - 1][2];

		fprintf(fileName, "%f,%f,%f,\n", x, y, z);
	}
}

void texturesToJs(FileContent *fd, FILE *fileName)
{
	fprintf(fileName, "function loadtextcoords() {\nreturn [\n");
	int nf = fd->numFaces;

	if (fd->numTexts > 0)
		for (int i = 0; i < nf; i++)
		{
			Face *f = fd->faces[i];
			textureToJs(fd, f, fileName);
		}

	fprintf(fileName, "];\n\n}\n\n");
}
void textureToJs(FileContent *fd, Face *f, FILE *fileName)
{
	FaceElement *e1 = f->firstElem;
	FaceElement *e2 = f->secondElem;
	FaceElement *e3 = f->thirdElem;
	FaceElement *e4 = NULL;
	if (f->numElems == 4)
		e4 = f->fourthElem;

	int v1 = e1->textureId;
	int v2 = e2->textureId;
	int v3 = e3->textureId;
	int v4 = -1;
	if (e4 != NULL)
		v4 = e4->textureId;
	float x, y;
	x = y = -1.0;

	x = fd->textures[v1 - 1][0];
	y = fd->textures[v1 - 1][1];

	fprintf(fileName, "%f,%f,\n", x, y);

	x = fd->textures[v2 - 1][0];
	y = fd->textures[v2 - 1][1];

	fprintf(fileName, "%f,%f,\n", x, y);

	x = fd->textures[v3 - 1][0];
	y = fd->textures[v3 - 1][1];

	fprintf(fileName, "%f,%f,\n", x, y);

	if (v4 != -1)
	{
		x = fd->textures[v4 - 1][0];
		y = fd->textures[v4 - 1][1];

		fprintf(fileName, "%f,%f,\n", x, y);
	}
}

void indexToJs(FileContent *fd, FILE *fileName)
{

	fprintf(fileName, "function loadvertexindices()\n{\nreturn [\n");
	Face *face = fd->faces[0];
	if (face->numElems == 3)
	{
		for (int i = 1; i <= fd->numFaces * 3; i += 3)
		{
			fprintf(fileName, "%d,%d,%d,\n", i - 1, i, i + 1);
		}
	}
	else
	{
		for (int i = 1; i <= fd->numFaces * 4; i += 4)
		{
			fprintf(fileName, "%d,%d,%d,%d,\n", i - 1, i, i + 1, i + 2);
		}
	}

	fprintf(fileName, "];\n}\n\n");
}
int *setOcc(int n)
{
	int *occupied = calloc(n, sizeof(int));
	for (int i = 0; i < n; i++)
		occupied[i] = -1;
	return occupied;
}

void widthToJs(PPM *ppm, FILE *fileName)
{
	fprintf(fileName, "function loadwidth() {\nreturn %ld;\n}\n\n", ppm->width);
}
void heightToJs(PPM *ppm, FILE *fileName)
{
	fprintf(fileName, "function loadheight() {\nreturn %ld;\n}\n\n", ppm->height);
}

void rgbToJs(PPM *ppm, FILE *fileName)
{
	fprintf(fileName, "function loadtexture(){ \n\nreturn (new Uint8Array(\n[\n");
	for (int i = 0; i < ppm->numReds; i++)
	{
		fprintf(fileName, "%ld,%ld,%ld,255,\n", ppm->reds[i], ppm->greens[i], ppm->blues[i]);
	}
	fprintf(fileName, "]\n));\n\n}\n\n");
}

void getdistance(FILE *fileName)
{
	fprintf(fileName, "function getdistance(){return -6.0;}\n\n");
}

void getVertexCount(FILE *fileName, FileContent *fc)
{
	Face *f = fc->faces[0];
	int n;
	if (f->numElems == 3)
		n = 3;
	else
		n = 4;
	fprintf(fileName, "function getVertexCount(){return %d;}\n\n", fc->numFaces * n);
}

void runA3(char *fileName)
{
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;

	Py_Initialize();
	pName = PyUnicode_DecodeFSDefault("readppm");

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL)
	{
		pFunc = PyObject_GetAttrString(pModule, "readppm");

		if (pFunc && PyCallable_Check(pFunc))
		{
			pArgs = PyTuple_New(1);
			pValue = Py_BuildValue("s", fileName);
			PyTuple_SetItem(pArgs, 0, pValue);
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			if (pValue != NULL)
			{

				PPM *ppm = newPPM(pValue);
				char fn[100];
				strcpy(fn, fileName);
				strcat(fn, ".obj");
				FileContent *fc = driver(fn);
				makeJavascript(fc, ppm);
				freePPM(ppm);
				deleteFileData(fc);
			}
			else
			{
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr, "Call failed\n");
				Py_Finalize();

				return;
			}
		}
		else
		{
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", "readppm");
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else
	{
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", fileName);
		Py_Finalize();

		return;
	}
	Py_Finalize();
}

int main(int argc, char *argv[])
{

	runA3(argv[1]);

	return 0;
}
