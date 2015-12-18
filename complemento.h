#ifndef COMPLEMENTO_H_INCLUDED
#define COMPLEMENTO_H_INCLUDED

#pragma once

struct AgrupLinear
{
    int dir, esqu, cima, baixo;
    bool qCimaDir, qCimaEsqu, qBaixoDir, qBaixoEsqu;
    bool rVdir, rVesqu, rHcima, rHbaixo;
};

struct Jogada
{
    int x, y;
    bool dir, esqu, cima, baixo;
    bool qV, qH;
    bool qCimaDir, qCimaEsqu, qBaixoDir, qBaixoEsqu;
    bool rVdir, rVesqu, rHcima, rHbaixo;
};

string Simplifica(bool** map, vector<Jogada>& melhorJogada, int L, int C)
{
    bool trivial= true;
    // Agrupamento Linear
    AgrupLinear** mapL= new AgrupLinear*[L];
    for(int i=0; i<L; i++)
        mapL[i]= new AgrupLinear[C];

    for(int i=0; i<L; i++)
        for(int j=0; j<C; j++)
            if(map[i][j] == 1)
            {
                // Vizinhos direita
                int vd= 0, ve= 0, vc=0, vb=0;
                for(int x=1; x<C; x++) { if(map[i][(j+x)%C] == 0) break; vd++; }
                // Vizinhos esquera
                for(int x=1, y=(j-x)<0?(C-1):(j-x); x<C; x++) { if(map[i][y] == 0) break; ve++; y--; if(y<0) y=C-1; }
                // Vizinhos cima
                for(int x=1, y=(i-x)<0?(L-1):(i-x); x<L; x++) { if(map[y][j] == 0) break; vc++; y--; if(y<0) y=L-1; }
                // Vizinhos baixo
                for(int x=1; x<L; x++) { if(map[(i+x)%L][j] == 0) break; vb++; }
                mapL[i][j]= AgrupLinear{vd, ve, vc, vb,
                                        false, false, false, false,
                                        false, false, false, false};
            }
            else
            {
                trivial= false;
                mapL[i][j]= AgrupLinear{0, 0, 0, 0,
                                        false, false, false, false,
                                        false, false, false, false};
            }
    if(trivial)
        return "1";
    // Agrupamento retangular
    for(int i=0; i<L; i++)
        for(int j=0; j<C; j++)
         if(map[i][j] == 1)
            {
                int dir=(j+1)%C, esqu= (j-1)<0?(C-1):(j-1), cima= (i-1)<0?(L-1):(i-1), baixo= (i+1)%L;

                // Agrupamento de 4 quadrados
                if(mapL[i][j].cima > 0 && mapL[i][dir].cima > 0) mapL[i][j].qCimaDir= true;
                if(mapL[i][j].cima > 0 && mapL[i][esqu].cima > 0) mapL[i][j].qCimaEsqu= true;
                if(mapL[i][j].baixo > 0 && mapL[i][dir].baixo > 0) mapL[i][j].qBaixoDir= true;
                if(mapL[i][j].baixo > 0 && mapL[i][esqu].baixo  > 0) mapL[i][j].qBaixoEsqu= true;

                // Agrupamento de 8 quadrados vertical
                if(mapL[i][j].cima == 3 && mapL[i][dir].cima == 3) mapL[i][j].rVdir= true;
                if(mapL[i][j].cima == 3 && mapL[i][esqu].cima == 3) mapL[i][j].rVesqu= true;
                // Agrupamento de 8 quadrados horizontal
                if(mapL[i][j].dir == 3 && mapL[cima][j].dir == 3) mapL[i][j].rHcima= true;
                if(mapL[i][j].esqu == 3 && mapL[baixo][j].esqu == 3) mapL[i][j].rHbaixo= true;
            }

    // Elimina possibilidades menores
    for(int i=0; i<L; i++)
        for(int j=0; j<C; j++)
        {
            if(mapL[i][j].rVdir || mapL[i][j].rVesqu || mapL[i][j].rHcima || mapL[i][j].rHbaixo)
            {
                mapL[i][j].qCimaDir=mapL[i][j].qCimaEsqu=mapL[i][j].qBaixoDir=mapL[i][j].qBaixoEsqu= false;
                mapL[i][j].baixo=mapL[i][j].cima=mapL[i][j].dir=mapL[i][j].esqu=0;
            }
            else if(mapL[i][j].qCimaDir || mapL[i][j].qCimaEsqu || mapL[i][j].qBaixoDir || mapL[i][j].qBaixoEsqu)
            {
                if(mapL[i][j].baixo != 3) mapL[i][j].baixo=0;
                if(mapL[i][j].cima != 3) mapL[i][j].cima= 0;
                if(mapL[i][j].dir != 3) mapL[i][j].dir= 0;
                if(mapL[i][j].esqu != 3) mapL[i][j].esqu= 0;
            }
            if(mapL[i][j].cima == 3) mapL[i][j].baixo= 0;
            if(mapL[i][j].dir == 3) mapL[i][j].esqu= 0;
        }
    // Procura melhor porta
    string** abcd= new string*[L];
    for(int i=0; i<L; i++)
        abcd[i]= new string[C];
    if(L == 4 && C == 4) //4
    {
        abcd[0][0]= "0000"; abcd[0][1]= "0001"; abcd[0][2]= "0011"; abcd[0][3]= "0010";
        abcd[1][0]= "0100"; abcd[1][1]= "0101"; abcd[1][2]= "0111"; abcd[1][3]= "0110";
        abcd[2][0]= "1100"; abcd[2][1]= "1101"; abcd[2][2]= "1111"; abcd[2][3]= "1110";
        abcd[3][0]= "1000"; abcd[3][1]= "1001"; abcd[3][2]= "1011"; abcd[3][3]= "1010";
    }
    if(L == 2 && C == 4) // 3
    {
        abcd[0][0]= "000"; abcd[0][1]= "001"; abcd[0][2]= "011"; abcd[0][3]= "010";
        abcd[1][0]= "100"; abcd[1][1]= "101"; abcd[1][2]= "111"; abcd[1][3]= "110";
    }
    if(L == 2 && C == 2) // 2
    {
        abcd[0][0]= "00"; abcd[0][1]= "01";
        abcd[1][0]= "10"; abcd[1][1]= "11";
    }
    if(L == 1 && C == 2) // 1
    {
        abcd[0][0]= "0"; abcd[0][1]= "1";
    }
    bool melhorAns= false, umaporvez= true;
    string melhor;
    AgrupLinear** tmapL= new AgrupLinear*[L];
    bool** tmap= new bool*[L];
    for(int i=0; i<L; i++)
    {
        tmapL[i]= new AgrupLinear[C];
        tmap[i]= new bool[C];
    }
    for(int i=0; i<L; i++)
        for(int j=0; j<C; j++)
            tmapL[i][j]= mapL[i][j];
    int prevCont= 0, iteracao=0;
    while(!melhorAns)
    {
        if(iteracao > 100000)
        {
            bool para;
            cout << "Numero de possibilidades muito alto, verificar todas as respostas pode demorar\nDeseja parar (0)nao, (1)sim: ";
            cin >> para;
            if(para)
                break;
        }
        int cont= 0;
        umaporvez=melhorAns= true;
        for(int i=0; i<L; i++) // Verifica se ainda há possibilidades
            for(int j=0; j<C; j++)
            {
                if(map[i][j] == 1)
                {
                    if(tmapL[i][j].qBaixoDir || tmapL[i][j].qBaixoEsqu || tmapL[i][j].qCimaDir || tmapL[i][j].qCimaEsqu ||
                       tmapL[i][j].rHbaixo || tmapL[i][j].rHcima || tmapL[i][j].rVdir || tmapL[i][j].rVesqu ||
                       tmapL[i][j].baixo != 0 || tmapL[i][j].cima != 0 || tmapL[i][j].dir != 0 || tmapL[i][j].esqu != 0)
                            melhorAns= false;
                    else cont++;
                }
                tmap[i][j]= map[i][j];
            }
        if(prevCont == cont) // deixa modificar proxima variavel
        for(int i=0; i<L; i++)
            for(int j=0; j<C; j++)
            {
                if(!tmapL[i][j].qBaixoDir && !tmapL[i][j].qBaixoEsqu && !tmapL[i][j].qCimaDir && !tmapL[i][j].qCimaEsqu &&
                    !tmapL[i][j].rHbaixo && !tmapL[i][j].rHcima && !tmapL[i][j].rVdir && !tmapL[i][j].rVesqu &&
                    tmapL[i][j].baixo == 0 && tmapL[i][j].cima == 0 && tmapL[i][j].dir == 0 && tmapL[i][j].esqu == 0
                    && cont > 0 && map[i][j] == 1) // reseta todas as possibilidades anteriores
                {
                    tmapL[i][j]= mapL[i][j];
                    cont--;
                }
                if(cont == 0) break;
            }
        prevCont= cont;
        string ans; // resposta local
        vector<Jogada> jogadas;
        for(int i=0; i<L; i++)
            for(int j=0; j<C; j++)
                if(tmap[i][j] == 1)
                {
                    int dir=(j+1)%C, esqu= (j-1)<0?(C-1):(j-1), cima= (i-1)<0?(L-1):(i-1), baixo= (i+1)%L;
                    bool oito= tmapL[i][j].rVdir || tmapL[i][j].rVesqu || tmapL[i][j].rHcima || tmapL[i][j].rHbaixo;
                    bool quatro= tmapL[i][j].qCimaDir || tmapL[i][j].qCimaEsqu || tmapL[i][j].qBaixoDir || tmapL[i][j].qBaixoEsqu;
                    vector<string> resp;
                    string port;
                    Jogada davez= {i, j,
                    false, false, false, false,
                    false, false,
                    false, false, false, false,
                    false, false, false, false};
                    if(oito) // Agrupamento de 8
                    {
                        if(tmapL[i][j].rVdir)
                        {
                            davez.rVdir= true;
                            for(int x=0; x<L; x++) { resp.push_back(abcd[(i+x)%L][j]); resp.push_back(abcd[(i+x)%L][dir]);
                                                    tmap[(i+x)%L][j]= 0; tmap[(i+x)%L][dir]= 0; }
                            if(umaporvez) { umaporvez=tmapL[i][j].rVdir= false; }
                        }
                        else if(tmapL[i][j].rVesqu)
                        {
                            davez.rVesqu= true;
                            for(int x=0; x<L; x++) { resp.push_back(abcd[(i+x)%L][j]); resp.push_back(abcd[(i+x)%L][esqu]);
                                                    tmap[(i+x)%L][j]= 0; tmap[(i+x)%L][esqu]= 0; }
                            if(umaporvez) { umaporvez=tmapL[i][j].rVesqu= false; }
                        }
                        else if(tmapL[i][j].rHcima)
                        {
                            davez.rHcima= true;
                            for(int x=0; x<C; x++) { resp.push_back(abcd[i][(j+x)%C]); resp.push_back(abcd[cima][(j+x)%C]);
                                                    tmap[i][(j+x)%C]= 0;  tmap[cima][(j+x)%C]= 0 ;}
                            if(umaporvez) { umaporvez=tmapL[i][j].rHcima= false; }
                        }
                        else if(tmapL[i][j].rHbaixo)
                        {
                            davez.rHbaixo= true;
                            for(int x=0; x<C; x++) { resp.push_back(abcd[i][(j+x)%C]); resp.push_back(abcd[baixo][(j+x)%C]);
                                                    tmap[i][(j+x)%C]= 0;  tmap[baixo][(j+x)%C]= 0; }
                            if(umaporvez) { umaporvez=tmapL[i][j].rHbaixo= false; }
                        }
                    }
                    else if(quatro || tmapL[i][j].cima == 3 || tmapL[i][j].dir == 3) // Agrupamento de 4
                    {
                        if(tmapL[i][j].cima == 3)
                        {
                            davez.qV= true;
                            for(int x=0; x<L; x++) { resp.push_back(abcd[(i+x)%L][j]);
                                                    tmap[(i+x)%L][j]= 0;}
                            if(umaporvez) { umaporvez=false; tmapL[i][j].cima= 0; }
                        }
                        else if(tmapL[i][j].dir == 3)
                        {
                            davez.qH= true;
                            for(int x=0; x<C; x++) { resp.push_back(abcd[i][(j+x)%C]);
                                                    tmap[i][(j+x)%C]= 0;}
                            if(umaporvez) { umaporvez=false; tmapL[i][j].dir= 0; }
                        }
                        else if(tmapL[i][j].qCimaDir)
                        {
                            davez.qCimaDir= true;
                            tmap[i][j]=tmap[cima][j]=tmap[i][dir]=tmap[cima][dir]=0;
                            resp.push_back(abcd[i][j]);
                            resp.push_back(abcd[cima][j]);
                            resp.push_back(abcd[i][dir]);
                            resp.push_back(abcd[cima][dir]);
                            if(umaporvez) { umaporvez=tmapL[i][j].qCimaDir= false; }
                        }
                        else if(tmapL[i][j].qCimaEsqu)
                        {
                            davez.qCimaEsqu= true;
                            tmap[i][j]=tmap[cima][j]=tmap[i][esqu]=tmap[cima][esqu]=0;
                            resp.push_back(abcd[i][j]);
                            resp.push_back(abcd[cima][j]);
                            resp.push_back(abcd[i][esqu]);
                            resp.push_back(abcd[cima][esqu]);
                            if(umaporvez) { umaporvez=tmapL[i][j].qCimaEsqu= false; }
                        }
                        else if(tmapL[i][j].qBaixoDir)
                        {
                            davez.qBaixoDir= true;
                            tmap[i][j]=tmap[baixo][j]=tmap[i][dir]=tmap[baixo][dir]=0;
                            resp.push_back(abcd[i][j]);
                            resp.push_back(abcd[baixo][j]);
                            resp.push_back(abcd[i][dir]);
                            resp.push_back(abcd[baixo][dir]);
                            if(umaporvez) { umaporvez=tmapL[i][j].qBaixoDir= false; }
                        }
                        else if(tmapL[i][j].qBaixoEsqu)
                        {
                            davez.qBaixoEsqu= true;
                            tmap[i][j]=tmap[baixo][j]=tmap[i][esqu]=tmap[baixo][esqu]=0;
                            resp.push_back(abcd[i][j]);
                            resp.push_back(abcd[baixo][j]);
                            resp.push_back(abcd[i][esqu]);
                            resp.push_back(abcd[baixo][esqu]);
                            if(umaporvez) { umaporvez=tmapL[i][j].qBaixoEsqu= false; }
                        }
                    }
                    else // Agrupamento duplo e unico
                    {
                        if(tmapL[i][j].cima > 0)
                        {
                            davez.cima= true;
                            tmap[i][j]=tmap[cima][j]=0;
                            resp.push_back(abcd[i][j]);
                            resp.push_back(abcd[cima][j]);
                            if(umaporvez) { umaporvez=false; tmapL[i][j].cima= 0; }
                        }
                        else if(tmapL[i][j].baixo > 0)
                        {
                            davez.baixo= true;
                            tmap[i][j]=tmap[baixo][j]=0;
                            resp.push_back(abcd[i][j]);
                            resp.push_back(abcd[baixo][j]);
                            if(umaporvez) { umaporvez=false; tmapL[i][j].baixo= 0; }
                        }
                        else if(tmapL[i][j].dir > 0)
                        {
                            davez.dir= true;
                            tmap[i][j]=tmap[i][dir]=0;
                            resp.push_back(abcd[i][j]);
                            resp.push_back(abcd[i][dir]);
                            if(umaporvez) { umaporvez=false; tmapL[i][j].dir= 0; }
                        }
                        else if(tmapL[i][j].esqu > 0)
                        {
                            davez.esqu= true;
                            tmap[i][j]=tmap[i][esqu]=0;
                            resp.push_back(abcd[i][j]);
                            resp.push_back(abcd[i][esqu]);
                            if(umaporvez) { umaporvez=false; tmapL[i][j].esqu= 0; }
                        }
                        else
                        {
                            tmap[i][j]=0;
                            resp.push_back(abcd[i][j]);
                        }
                    }
                    if(!resp.empty()) // Cria porta
                    {
                        bool A_= true, B_= true, C_= true, D_= true;
                        for(unsigned int k=0; k<resp.size(); k++)
                        {
                            if(resp[0].at(0) != resp[k].at(0)) A_= false;
                            if(L > 1 && C > 1)
                            if(resp[0].at(1) != resp[k].at(1)) B_= false;
                            if(L > 1 && C > 2)
                            if(resp[0].at(2) != resp[k].at(2)) C_= false;
                            if(L > 2 && C > 2)
                            if(resp[0].at(3) != resp[k].at(3)) D_= false;
                        }
                        if(A_ && resp[0].at(0) == '0') port+="!A";
                        if(A_ && resp[0].at(0) == '1') port+="A";
                        if(L > 1 && C > 1)
                        {
                            if(B_ && resp[0].at(1) == '0') port+="!B";
                            if(B_ && resp[0].at(1) == '1') port+="B";
                        }
                        if(L > 1 && C > 2)
                        {
                            if(C_ && resp[0].at(2) == '0') port+="!C";
                            if(C_ && resp[0].at(2) == '1') port+="C";
                        }
                        if(L > 2 && C > 2)
                        {
                            if(D_ && resp[0].at(3) == '0') port+="!D";
                            if(D_ && resp[0].at(3) == '1') port+="D";
                        }
                        jogadas.push_back(davez);
                        if(ans.empty()) ans+=port;
                        else ans+="+"+port;
                    }
                }
        if(melhor.empty())
        {
            melhor= ans;
            melhorJogada= jogadas;
        }
        else if(ans.size() < melhor.size()) // verifica se é melhor resposta
        {
            melhorJogada= jogadas;
            melhor= ans;
        }
        iteracao++;
    }
    cout << "Agrupamentos\n";
    for(unsigned int i=0; i<melhorJogada.size(); i++)
    {
        cout << "pos " << melhorJogada[i].x << " " << melhorJogada[i].y << " ";
        if(melhorJogada[i].dir) cout << "Direita";
        else if(melhorJogada[i].esqu) cout << "Esquerda";
        else if(melhorJogada[i].cima) cout << "Cima";
        else if(melhorJogada[i].baixo) cout << "Baixo";
        else if(melhorJogada[i].qV) cout << "Quadrado Vertical";
        else if(melhorJogada[i].qH) cout << "Quadrado Horizontal";
        else if(melhorJogada[i].qCimaDir) cout << "Quadrado Cima Direita";
        else if(melhorJogada[i].qCimaEsqu) cout << "Quadrado Cima Esquerda";
        else if(melhorJogada[i].qBaixoDir) cout << "Quadrado Baixo Direita";
        else if(melhorJogada[i].qBaixoEsqu) cout << "Quadrado Baixo Esquerda";
        else if(melhorJogada[i].rVdir) cout << "Retangulo Vertical Direita";
        else if(melhorJogada[i].rVesqu) cout << "Retangulo Vertical Esquerda";
        else if(melhorJogada[i].rHcima) cout << "Retangulo Horizontal cima";
        else if(melhorJogada[i].rHbaixo) cout << "Retangulo Horizontal Baixo";
        else cout << "Unico";
        cout << endl;
    }
    cout << "Numero de iteracoes : " << iteracao << endl << endl;
    return (melhor.empty())?"0":melhor;
}

#endif // COMPLEMENTO_H_INCLUDED
