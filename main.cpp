#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>
using namespace std;

#include "complemento.h"

float width= 640, heigth= 480, ar= width/heigth;

class Cor
{
    float R, G, B;
public:
    Cor() {}
    Cor(float R, float G, float B): R(R), G(G), B(B) {}
    void Reseta()
    {
        R=G=B=0;
    }
    void Adiciona(Cor y)
    {
        R+=y.R;
        G+=y.G;
        B+=y.B;
    }
    friend class QuadradoCarnot;
};


class QuadradoCarnot
{
    float x, y, len;
    vector<Cor> cor;
    int estado;
public:
    int Estado() const { return estado; }
    void Inicializa(float x_, float y_, float len_, int estado_)
    {
        x= x_;
        y= y_;
        len= len_;
        estado= estado_;
        cor.push_back(Cor(0,0,0));
        ResetaCor();
    }
    void ResetaCor()
    {
        cor.clear();
        cor.push_back(Cor(0,0,0));
    }
    void AddCor(Cor y)
    {
        cor.push_back(y);
//        cor.Adiciona(y);
    }
    void Desenha()
    {
        float alpha= 0.7;
        for(auto &c:cor)
        {
            glPushMatrix();
                glColor4f(c.R,c.G,c.B,alpha);
                glTranslatef(x, y, 0);
                glBegin(GL_QUADS);
                    glVertex2f(-len, -len);
                    glVertex2f(len, -len);
                    glVertex2f(len, len);
                    glVertex2f(-len, len);
                glEnd();
            glPopMatrix();
            alpha+=0.02;
        }

        glPushMatrix();
            glColor4f(1,1,1,1);
            glTranslatef(x-len/2.5, y+len/2.0, 0);
            glScalef(0.0012, -0.001, 0);
            if(estado == 1)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, '1');
            else
                glutStrokeCharacter(GLUT_STROKE_ROMAN, '0');
        glPopMatrix();
    }
    void MudaEstado(float mx, float my)
    {
        if(mx>(x-len) && mx<(x+len) && my>(y-len) && my<(y+len))
        {
            estado= (estado == 1)?0:1;
        }
    }
};

string porta;
bool** map;
QuadradoCarnot mapa[4][4];

void Reshape(int w, int h)
{
    ar= (float)w/h;
    width= w; heigth= h;
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,ar,1,0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Texto(const char* txt, float x, float y)
{
    glPushMatrix();
        glColor4f(0,0,0, 1);
        glTranslatef(x, y, 0);
        glScalef(0.0005, -0.0005, 0);
        while(*txt)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *txt++);
    glPopMatrix();
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            mapa[i][j].Desenha();
        }
    }
    Texto("CD", 0.14, 0.06);
    Texto("00", 0.28, 0.06);
    Texto("01", 0.48, 0.06);
    Texto("11", 0.68, 0.06);
    Texto("10", 0.88, 0.06);
    Texto("AB", 0.1, 0.12);
    Texto("00", 0.1, 0.22);
    Texto("01", 0.1, 0.42);
    Texto("11", 0.1, 0.62);
    Texto("10", 0.1, 0.82);

    Texto(porta.c_str(), 0.22, 1);
    glutSwapBuffers();
}

void Mouse(int butao, int estado, int x, int y)
{
    float mx= x/width*ar, my= y/heigth;
    if(butao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN)
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                mapa[i][j].MudaEstado(mx, my);
                map[i][j]= mapa[i][j].Estado();
                glutPostRedisplay();
            }
        }
    }
    if(butao == GLUT_RIGHT_BUTTON && estado == GLUT_DOWN)
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                mapa[i][j].ResetaCor();
            }
        }
        vector<Jogada> X;
        porta= Simplifica(map, X, 4, 4);

        Cor cores[16]= {Cor(1,0,0), Cor(0,1,0), Cor(0,0,1),
        Cor(1,1,0), Cor(1,0,1), Cor(0,1,1), Cor(1,1,1)};
        for(int k=0; k<X.size(); k++)
        {
            int i= X[k].x, j= X[k].y;
            int dir=(j+1)%4, esqu= (j-1)<0?3:(j-1), cima= (i-1)<0?3:(i-1), baixo= (i+1)%4;
            mapa[i][j].AddCor(cores[k]);
            if(X[k].dir) { mapa[i][dir].AddCor(cores[k]); }
            if(X[k].esqu) { mapa[i][esqu].AddCor(cores[k]); }
            if(X[k].cima) { mapa[cima][j].AddCor(cores[k]); }
            if(X[k].baixo) { mapa[baixo][j].AddCor(cores[k]); }
            if(X[k].qCimaDir) { mapa[cima][j].AddCor(cores[k]); mapa[i][dir].AddCor(cores[k]); mapa[cima][dir].AddCor(cores[k]);}
            if(X[k].qCimaEsqu) { mapa[cima][j].AddCor(cores[k]); mapa[i][esqu].AddCor(cores[k]); mapa[cima][esqu].AddCor(cores[k]);}
            if(X[k].qBaixoDir) { mapa[baixo][j].AddCor(cores[k]); mapa[i][dir].AddCor(cores[k]); mapa[baixo][dir].AddCor(cores[k]);}
            if(X[k].qBaixoEsqu) {mapa[baixo][j].AddCor(cores[k]); mapa[i][esqu].AddCor(cores[k]); mapa[baixo][esqu].AddCor(cores[k]);}
            if(X[k].qV) {for(int x=1; x<4; x++) mapa[(i+x)%4][j].AddCor(cores[k]); }
            if(X[k].qH) {for(int x=1; x<4; x++) mapa[i][(j+x)%4].AddCor(cores[k]); }
            if(X[k].rVdir) {for(int x=0; x<4; x++)  { mapa[(i+x)%4][j].AddCor(cores[k]); mapa[(i+x)%4][dir].AddCor(cores[k]); } }
            if(X[k].rVesqu) {for(int x=0; x<4; x++) { mapa[(i+x)%4][j].AddCor(cores[k]); mapa[(i+x)%4][esqu].AddCor(cores[k]); } }
            if(X[k].rHcima) {for(int x=0; x<4; x++) { mapa[i][(j+x)%4].AddCor(cores[k]); mapa[cima][(j+x)%4].AddCor(cores[k]); } }
            if(X[k].rHbaixo){for(int x=0; x<4; x++) { mapa[i][(j+x)%4].AddCor(cores[k]); mapa[baixo][(j+x)%4].AddCor(cores[k]); } }
        }
        glutPostRedisplay();
    }
}

void Time(int t)
{
//    glutTimerFunc(30, Time, t);
}

int main(int argc, char** argv)
{
    map= new bool*[4];
    for(int i=0; i<4; i++)
        map[i]= new bool[4];
    float x=0.32, y=0.2, len= 0.1;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            map[i][j]= 0;
            mapa[i][j].Inicializa(x,y,len,0);
            x+=2*len+0.01;
        }
        x=0.32;
        y+=2*len+0.01;
    }
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Mapa");

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutTimerFunc(30, Time, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1,1,1,1);
    glutMainLoop();
}
