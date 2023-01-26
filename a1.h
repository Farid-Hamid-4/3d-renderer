#ifndef __A1__
#define __A1__

#include <Python/Python.h>

struct faceelement
{
	int vertexId;
	int textureId;
	int normalId;
};

typedef struct faceelement FaceElement;

struct face
{
	FaceElement *firstElem;
	FaceElement *secondElem;
	FaceElement *thirdElem;
	FaceElement *fourthElem;
	int numElems;
};

typedef struct face Face;

struct filecontent
{
	float **vertices;
	int numVerts;
	float **textures;
	int numTexts;
	float **normals;
	int numNorms;
	Face **faces;
	int numFaces;
};

typedef struct filecontent FileContent;

struct ppm
{
	char code[3];
	long width;
	long height;
	long depth;
	long *reds;
	long numReds;
	long *greens;
	long numGreens;
	long *blues;
	long numBlues;
};
typedef struct ppm PPM;

char *getLine(FILE *fp);

char **tokenize(char *string, char *delims);

void printStrings(char **strings);

void freeStrings(char **strings);

/**CONSTRUCTORS**/

FileContent *makeFileContent(void);
FileContent *driver(char *fileName);

FaceElement *parseFaceElement(char *string);
FaceElement *makeFaceElement(int vId, int tId, int nId);

Face *makeFace(char *string);

/**setters**/
void addVertex(FileContent *fc, float x, float y, float z);

void addTexture(FileContent *fc, float x, float y);

void addNormal(FileContent *fc, float x, float y, float z);

void addElement(Face *face, int v, int vt, int vn);

void addFace(FileContent *fc, char *face);

int parser(FileContent *fc, FILE *fp);

/**memory clean up**/

void deleteFaceElement(void *d);
void deleteFace(void *d);
void deleteFileData(void *d);

/**printers**/

char *printFace(FileContent *fc, Face *f, int n);
char *printFileData(FileContent *fc);

/***validating functions***/

int validVertexString(char *s);

int validTextureString(char *s);

int validNormalString(char *s);

int validFaceString(char *s);

int validateElemString(char *s);

/*****A2 FUNCTIONS*****/

// get rgb values from python list
long **getRGBvalues(PyObject *list, int *rLen, int *gLen, int *bLen);

PPM *newPPM(PyObject *pValue);
void freePPM(PPM *ppm);
void printPPM(PPM *ppm);

void loadvertices(FILE *fp, FileContent *fc);
void loadnormals(FILE *fp, FileContent *fc);
void loadvertices(FILE *fp, FileContent *fc);
void loadvertices(FILE *fp, FileContent *fc);
void loadvertices(FILE *fp, FileContent *fc);

void makeJavascript(FileContent *fc, PPM *ppm);

void objToJs(FILE *fileName);
void ppmToJs(FILE *fileName);

void verticicesToJs(FileContent *fd, FILE *fileName);
void vertexToJs(FileContent *fd, Face *f, FILE *fileName);

void normalsToJs(FileContent *fd, FILE *fileName);
void normalToJs(FileContent *fd, Face *f, FILE *fileName);

void texturesToJs(FileContent *fd, FILE *fileName);
void textureToJs(FileContent *fd, Face *f, FILE *fileName);

void indexToJs(FileContent *fd, FILE *fileName);
int *setOcc(int n);

void rgbToJs(PPM *ppm, FILE *fileName);
void widthToJs(PPM *ppm, FILE *fileName);

void heightToJs(PPM *ppm, FILE *fileName);

void runA3(char *fileName);

void getdistance(FILE *fileName);

void getVertexCount(FILE *fileName, FileContent *fc);

#endif