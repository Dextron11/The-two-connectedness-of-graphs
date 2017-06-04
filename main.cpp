#include <iostream>
#include <cstring>
#include <fstream>

int kk=1;
using namespace std;
typedef struct L *Lref; // Тип: указатель на заголовочный узел.
typedef struct T *Tref; // Тип: указатель на дуговой узел.

//Описание типа заголовочного узла.
typedef struct L
{
    int Key; //Имя заголовочного узла.
    int Count; //Количество предшественников.
    int WGN; //Характеристика узла.
    Tref Trail; //Указатель на список смежности.
    Lref Next; //Указатель на следующий узел в списке заголовочных узлов.
} Leader;

//Описание типа дугового узла.
typedef struct T
{
    Lref Id;
    Tref Next;
} Trailer;

//Описание типа узла стека.
typedef Lref TipElement;
typedef struct Zveno *svqz;
typedef struct Zveno
{
    TipElement Element1;
    TipElement Element2;
    svqz Sled;
} St;

int L[30]; //Количество  элементов  в  массиве L
//совпадает с количеством вершин графа.
int num=0;

class Spisok
{
private:
    Lref Head; //Указатель на голову списка заголовочных узлов.
    Lref Tail; //Указатель на фиктивный элемент
    //в конце списка заголовочных узлов.
    svqz Stack; //Указатель на рабочий стек.
    void SearchGraph (int, Lref *);
    void W_S (svqz *, TipElement, TipElement);
    void UDALENIE (svqz *, TipElement *, TipElement *);
public:
    Spisok() {//Инициализация списка заголовочных узлов.
        Head = Tail =  new (Leader); Stack = NULL;}
    Lref GetHead() { return Head; }
    Lref &GetHead1() { return Head; }
    Lref GetTail() { return Tail; }
    void MakeGraph ();
     void TwoLink (Lref, Lref);
};



void readtxt(float **&arr, char *name, int &i)
{
    FILE *f;
    f = fopen (name,"r");
  while (feof(f)==0)
  {
      arr[0] = (float*)realloc(arr[0], (i + 1) * sizeof(float));
      fscanf(f,"%f",&arr[0][i]);
      arr[1] = (float*)realloc(arr[1], (i + 1) * sizeof(float));
      fgetc(f);
      fscanf(f,"%f",&arr[1][i]);
      i++;
  }
    fclose(f);
}
void writegv(float **&arr,char *name,int &k)
{
    ofstream out(name);
    out << "graph s {\n";
    for (int i=0;i<k;i++)
    {
        if((arr[0][i])!=0)
        out<<arr[0][i]<<"--"<<arr[1][i]<<";\n";
    }
    out<<"}";
    out.close ();
}

int main ()
{
    Spisok A;
    Lref SuperHead;//Заглавное звено списка заголовочных узлов.
    Lref t; //Рабочий указатель для перемещения
    //по списку заголовочных звеньев.
    //Построение графа и вывод его структуры Вирта.
    A.MakeGraph ();
    //Нахождение компонент двусвязности графа.
    t = A.GetHead();
    while (t!=A.GetTail())
    { (*t).WGN = 0; t = (*t).Next;}
    //Построение заглавного звена списка заголовочных звеньев.
    SuperHead = new (Leader);
    SuperHead->Key = 0; SuperHead->Next = A.GetHead();
    A.GetHead1() = SuperHead;
    t = A.GetHead()->Next;
    while (t!=A.GetTail())
    {
        if ( t->WGN==0 ) A.TwoLink (t,A.GetHead());
        t = t->Next;
    }
    int k=0;
    float **arr=NULL;
    arr = (float**)malloc(2 * sizeof(float*));
    arr[0]=(float*)malloc(1*sizeof(float*));
    arr[1]=(float*)malloc(1*sizeof(float*));
    readtxt(arr,"input.txt",k);
    writegv(arr,"inGv.gv",k);
    system("circo -Tpng inGv.gv -output.png");
    free(arr[0]);
    free(arr[1]);
    k=0;
    arr[0]=(float*)malloc(1*sizeof(float*));
    arr[1]=(float*)malloc(1*sizeof(float*));
    readtxt(arr,"output.txt",k);
    writegv(arr,"outGv.gv",k);
    system("circo -Tpng outGv.gv -output2.png");
    return 0;
}

void Spisok::SearchGraph (int w, Lref *h)
//Функция возвращает указатель на заголовочный узел
//с ключом w в графе, заданном структурой Вирта с указателем Head.
{
    *h = Head; (*Tail).Key = w;
    while ((**h).Key!=w) *h = (**h).Next;
    if (*h==Tail)
        //В списке заголовочных узлов нет узла с ключом w.
        //Поместим его в конец списка Head.
    { Tail = new (Leader); (**h).Count = 0;
        (**h).Trail = NULL; (**h).Next = Tail; }
}

void Spisok::MakeGraph ()
//Функция возвращает указатель Head на структуру
//Вирта, соответствующую ориентированному графу.
{
    int x,y;
    Lref p,q; //Рабочие указатели.
    Tref t,r; //Рабочие указатели.
    bool Res; //Флаг наличия дуги.

    FILE *file;
    file = fopen("input.txt", "r");
    while (feof(file)==0)
    {
        fscanf(file,"%d",&x);
        fgetc(file);
        fscanf(file,"%d",&y);
        //Определим, существует ли в графе дуга (x,y)?
        SearchGraph (x, &p); SearchGraph (y,&q);
        r = (*p).Trail; Res = false;
        while ((r!=NULL)&&(!Res))
            if ((*r).Id==q) Res = true;
            else r = (*r).Next;
        if (!Res) //Если дуга отсутствует, то поместим её в граф.
        { t = new (Trailer); (*t).Id = q;
            (*t).Next = (*p).Trail; (*p).Trail = t; (*q).Count++; }
    }
}

void Spisok::W_S (svqz *stk, TipElement el1, TipElement el2)
//Помещение el1 и el2 в стек stk.
{
    svqz q=new (St);
    (*q).Element1 = el1; (*q).Element2 = el2;
    (*q).Sled = *stk; *stk = q;
}

void Spisok::UDALENIE (svqz *stk, TipElement *klad1, TipElement *klad2)
//Удаление звена из стека, заданного указателем *stk.
//Значение информационного поля удаляемого звена сохраня-
//ется в параметрах klad1 и klad2.
{
    svqz q;

    if (*stk==NULL) cout<<"Попытка выбора из пустого стека!\n";
    else
    { *klad1 = (**stk).Element1;
        *klad2 = (**stk).Element2;
        q = *stk; *stk = (**stk).Sled; delete q; }
}

void Spisok::TwoLink (Lref r, Lref p)
//Поиск в глубину, начиная с вершины r->Key и полагая,
//что p является указателем на отца вершины r->Key.
{
    Tref t;
    Lref e1,e2;
    ofstream fout("output.txt",ios_base::app);
    num++; r->WGN = num; L[r->Key] = r->WGN;
    t = r->Trail;
    while ( t != NULL )
    {
        if ( t->Id->WGN==0 )
        {
            W_S (&Stack,r,t->Id);
            TwoLink (t->Id,r);
            if ( L[r->Key] > L[t->Id->Key] )
                L[r->Key] = L[t->Id->Key];
            if ( L[t->Id->Key] >= r->WGN )
            {
                //Выписать ребра компоненты двусвязности.
                do {
                    UDALENIE (&Stack,&e1,&e2);
                    fout<<e1->Key<<"."<<kk<<" "<<e2->Key<<"."<<kk<<"\n";
           }
                while (!((e1->Key==r->Key && e2->Key==t->Id->Key) || (e1->Key==t->Id->Key && e2->Key==r->Key)));
                kk++;
            }
        }
        else
        if  (t->Id->Key != p->Key &&  r->WGN > t->Id->WGN)
        {
            W_S (&Stack,r,t->Id);
            if ( L[r->Key] > t->Id->WGN )
                L[r->Key] = t->Id->WGN;
        }
        t = t->Next;
    }
}
