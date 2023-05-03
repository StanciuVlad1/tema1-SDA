/* STANCIU Vlad-Mihai - 311C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Definirea structurii pentru nodurile din interiorul structurii Banda Magica
typedef struct BandaMagicaNod
{
    struct BandaMagicaNod *next;
    struct BandaMagicaNod *prev;
    char data;
} BandaMagicaNod;
//Definirea structurii de Banda Magica
typedef struct BandaMagica
{
    BandaMagicaNod *sentinel;
    BandaMagicaNod *deget;
} BandaMagica;
//Definirea structurii pentru nodurile din interiorul celor doua stive
typedef struct StackNode
{
    struct BandaMagicaNod *data;
    struct StackNode *next;
} StackNode;
//Definirea structurii de stiva
typedef struct Stack
{
    StackNode *top;
} Stack;
//Definirea structurii pentru nodurile din interiorul cozii
typedef struct QueueNode
{
    char data[18];
    struct QueueNode *next;
} QueueNode;
//Definirea structurii pentru coada
typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
} Queue;

//Initializeaza Banda Magica
void Initializare_BandaMagica(BandaMagica **Banda_Magica);
//Functia de MOVE_LEFT
void Move_left(BandaMagica *Banda_Magica);
//Functia de MOVE_RIGHT
void Move_right(BandaMagica *Banda_Magica);
//Functia de MOVE_LEFT_CHAR
void Move_left_char(BandaMagica *Banda_Magica, char c, FILE *output);
//Functia de MOVE_RIGHT_CHAR
void Move_right_char(BandaMagica *Banda_Magica, char c);
//Functia de WRITE C
void Write_C(BandaMagica *Banda_Magica, char c);
//Functia de INSERT_LEFT
void Insert_left(BandaMagica *Banda_Magica, char c, FILE *output);
//Functia de INSERT_RIGHT
void Insert_right(BandaMagica *Banda_Magica, char c);
//Functia de SHOW_CURRENT
void Show_current(BandaMagica *Banda_Magica, FILE *output);
//Functia de SHOW
void Show(BandaMagica *Banda_Magica, FILE *output);
//Initializeaza coada
void Initializare_Queue(Queue **q);
//Adauga nod nou in coada
void AddQueue(Queue *q, char s[]);
//Sterge primul nod din coada
void RemoveQueue(Queue *q);
//Initializeaza stiva
void Initializare_Stack(Stack **s);
//Adauga nod nou in stiva
void PushStack(Stack *s, BandaMagicaNod *p);
//Sterge ultimul nod din stiva
void PopStack(Stack *s);
//Functia de EXECUTE
void execute(Queue *q, Stack *UndoStack, Stack *RedoStack, BandaMagica *Banda_Magica, FILE *output);
//Citeste datele din fisierul de intrare,rezolva problema si trece solutia in fisierul de iesire
void rezolvare(BandaMagica *Banda_Magica, Queue *q, Stack *UndoStack, Stack *RedoStack, FILE *in, FILE *out);
//Elibereaza memoria ocupata de o structura de tip Stack
void free_Stack(Stack *s);
//Elibereaza memoria ocupata de o structura de tip Queue
void free_Queue(Queue *q);
//Elibereaza memoria ocupata de o structura de tip BandaMagica
void free_BandaMagica(BandaMagica *Banda_Magica);
int main()
{ //Declaram si initializam fisierul de intrare, respectiv de iesire
    FILE *in, *out;
    in = fopen("tema1.in", "r");
    //Verificam daca tema1.in a fost deschis corect
    if (in == NULL)
    { //Afisam un mesaj in caz contrar
        printf("Eroare la deschidere fisierului de intrare\n");
        exit(1);
    }
    //Verificam daca tema1.out a fost deschis corect
    out = fopen("tema1.out", "w");
    if (out == NULL)
    { //Afisam un mesaj in caz contrar
        printf("Eroare la deschidere fisierului de iesire\n");
        exit(1);
    }
    //Declaram Banda_Magica
    BandaMagica *Banda_Magica;
    //Declaram coada q pentru a stoca comenzile UPDATE necesare
    Queue *q;
    //Declaram stivele UndoStack si RedoStack pentru a stoca pointerii necesari
    Stack *UndoStack, *RedoStack;
    //Initializam banda
    Initializare_BandaMagica(&Banda_Magica);
    //Initializam coada
    Initializare_Queue(&q);
    //Initializam stiva pentru UNDO
    Initializare_Stack(&UndoStack);
    //Initializam stiva pentru REDO
    Initializare_Stack(&RedoStack);
    //Apelam functia de rezolvare propriu-zisa a porblemei
    rezolvare(Banda_Magica, q, UndoStack, RedoStack, in, out);
    //Eliberam memoria alocata stivei pentru UNDO
    free_Stack(UndoStack);
    //Eliberam memoria alocata stivei pentru REDO
    free_Stack(RedoStack);
    //Eliberam memoria alocata cozii pentru comenzile UPDATE
    free_Queue(q);
    //Eliberam memoria alocata pentru Banda_Magica
    free_BandaMagica(Banda_Magica);
    //Inchidem corespunzator fisierul de intrare
    fclose(in);
    //Inchidem corespunzator fisierul de iesire
    fclose(out);

    return 0;
}

void Initializare_BandaMagica(BandaMagica **Banda_Magica)
{ //Alocam dinamic memorie pentru Banda_Magica
    (*Banda_Magica) = (BandaMagica *)malloc(sizeof(BandaMagica));
    BandaMagicaNod *p;
    /*Alocam dinamic memore nodului p,initializat anterior,care va reprezenta
      primul nod dupa santinela, cu informatia #
    */
    p = (BandaMagicaNod *)malloc(sizeof(BandaMagicaNod));
    //Alocam dinamic memorie pentru nodul santinala, declarat sentinel
    (*Banda_Magica)->sentinel = (BandaMagicaNod *)malloc(sizeof(BandaMagicaNod));
    //Punem informatia de tip char, "#", in primul nod
    p->data = '#';
    //Ii cream cele doua legaturi next, respectiv prev
    p->next = NULL;
    p->prev = (*Banda_Magica)->sentinel;
    //Cream clee doua legaturi next,respectiv prev pentru santinela
    (*Banda_Magica)->sentinel->prev = NULL;
    (*Banda_Magica)->sentinel->next = p;
    //Pozitionam degetul sa pointeze la primul nod din banda magica,respectiv p
    (*Banda_Magica)->deget = p;
}

void Move_left(BandaMagica *Banda_Magica)
{ /*Verificam ca degetul sa nu se afle pe prima pozitie in banda prin a
    verifica daca nodul de inaintea lui este diferit de santinela.In caz
    afirmativ, degetul pointeaza cu o pozitie la stanga.In caz contrar,
    functia nu va face nicio modificare.
    */
    if (Banda_Magica->deget->prev != Banda_Magica->sentinel)
        Banda_Magica->deget = Banda_Magica->deget->prev;
}

void Move_right(BandaMagica *Banda_Magica)
{ /*Verificam daca degetul se afla pe ultima pozitie din banda prin a
    verifica daca deget->next pointeaza catre NULL.In caz afirmativ,
    cream un nou nod
    */
    if (Banda_Magica->deget->next == NULL)
    { //Alocam dinamic memorie pentru noul nod
        BandaMagicaNod *nou = (BandaMagicaNod *)malloc(sizeof(BandaMagicaNod));
        //Find ultimul nod din banda, nou->next va pointa catre NULL
        nou->next = NULL;
        //Nodul anterior va fi degetul actual al benzii
        nou->prev = Banda_Magica->deget;
        //Punem in informatia de tip char a noului nod, caracterul "#"
        nou->data = '#';
        //Legam deget de noul nod adaugat
        Banda_Magica->deget->next = nou;
        //Pozitionam degetul sa pointeze cater noul nod
        Banda_Magica->deget = nou;
    }
    else
    { /*In caz contrar, doar pozitionam degetul sa pointeze cu o pozitie
        la dreapta*/
        Banda_Magica->deget = Banda_Magica->deget->next;
    }
}

void Move_left_char(BandaMagica *Banda_Magica, char c, FILE *output)
{ //Declaram un iterator pentru a parcurge banda
    BandaMagicaNod *p;
    for (p = Banda_Magica->deget; p != Banda_Magica->sentinel; p = p->prev)
    { //Verificam daca intalnim elemntul cautat
        if (p->data == c)
        { /*In caz afirmativ, degetul va pointa catre pozitia elementului
            gasit
            */
            Banda_Magica->deget = p;
            //Acest break ne va scoate din for, pentru o eficienta sporita
            break;
        }
    }
    /*In cazul in care iteratorul a ajuns la santinela, inseamna ca nu s-a
    gasit caracterul cautat, caz in care vom afisa in fisierul de output,
    mesajul "ERROR"
    */
    if (p == Banda_Magica->sentinel)
        fprintf(output, "ERROR\n");
}

void Move_right_char(BandaMagica *Banda_Magica, char c)
{ //Declaram un iterator pentru a parcurge banda
    BandaMagicaNod *p;
    for (p = Banda_Magica->deget; p->next != NULL; p = p->next)
        //Verificam daca intalnim elemntul cautat
        if (p->data == c)
        {
            /*In caz afirmativ, degetul va pointa catre pozitia elementului
            gasit
            */
            Banda_Magica->deget = p;
            //Acest break ne va scoate din for, pentru o eficienta sporita
            break;
        }
    /*Am luat aces caz separat pentru a ne pozitiona pe ultima pozitie din banda
    in cazul in care va trebui sa adaugam un nou nod la finalul benzii si
    pentru a putea realiza legaturile necesare intre noduri.In prima instanta,
    verificam daca am ajuns pe ultima pozitie din banda.
    */
    if (p->next == NULL)
    { //In caz afirmativ verificam daca nodul contine caracterul cautat
        if (p->data == c)
        { /*In caz afirmativ, pozitionam degetul sa pointeze catre ultimul
            nod*/
            Banda_Magica->deget = p;
        }
        else
        { /*In caz contrar, adaugam un nod nou,si modificam pozitia degetului
            sa pointeze catre noul nod creat */
            //Alocam dinamic memorie pentru noul nod
            BandaMagicaNod *nou = (BandaMagicaNod *)malloc(sizeof(BandaMagicaNod));
            /*Realizam legaturile cu NULL pentru nou->next, respectiv cu nodul
            de pe ultima pozitie pentru nou->prev*/
            nou->next = NULL;
            nou->prev = p;
            //Punem in informatia de tip char a noului nod, caracterul "#"
            nou->data = '#';
            //Legam ultimul nod de noul nod creat prin p->next=nou
            p->next = nou;
            //Modificam pozitia degetului sa pointeze la noul nod creat
            Banda_Magica->deget = nou;
        }
    }
}

void Write_C(BandaMagica *Banda_Magica, char c)
{ /*Inlocuim informatia curenta din nodul la care pointeaza degetul, cu 
    parametrul char c primit de functie 
    */
    Banda_Magica->deget->data = c;
}

void Insert_left(BandaMagica *Banda_Magica, char c, FILE *output)
{ /*Luam in calcul daca degetul pointeaza catre primul nod din lista,
    verificand daca nodul anterior degetului este santinela.In caz afirmativ,
    afisam mesajul "ERROR", deoarece nu se poate efectua inserarea.In caz
    negativ, cream un nod nou ce primeste ca informatie parametrul char c.
    */
    if (Banda_Magica->deget->prev == Banda_Magica->sentinel)
        fprintf(output, "ERROR\n");
    else
    { /*Alocam dinamic memorie pentru noul nod,in cazul in care operatia este
        de inserare este posibila
        */
        BandaMagicaNod *nou = (BandaMagicaNod *)malloc(sizeof(BandaMagicaNod));
        /*Punem in informatia de tip char a noului nod, caracterul char c
        primit ca parametru de functie */
        nou->data = c;
        /*Realizam legaturile noului nod, legatura de next cu degetul actual
        al benzii pentru ca nodul se afla la stanga degetului,iar legatura de
        prev cu nodul anterior degetului actual,deoarece nodul nou se va afla
        in dreapta acestuia.
        */
        nou->next = Banda_Magica->deget;
        nou->prev = Banda_Magica->deget->prev;
        /*Acutalizam legatura de next a nodului anterior degetului actual cu
        noul nod*/
        Banda_Magica->deget->prev->next = nou;
        //Acutalizam legatura de prev a degetului actual cu noul nod
        Banda_Magica->deget->prev = nou;
        //Modificam pozitia degetului sa pointeze la noul nod creat
        Banda_Magica->deget = nou;
    }
}

void Insert_right(BandaMagica *Banda_Magica, char c)
{
    //Alocam dinamic memorie pentru noul nod
    BandaMagicaNod *nou = (BandaMagicaNod *)malloc(sizeof(BandaMagicaNod));
    /* Stabilim legaturile noului nod inserat.Pentru nou->next vom atribui
    pointerul urmator degetului actual, iar pentru nou->prev vom atribui
    degetul actual.
    */
    nou->prev = Banda_Magica->deget;
    nou->next = Banda_Magica->deget->next;
    /*Ne punem problema daca degetul nu se afla pe ultima pozitie in banda.In
    acest caz, vom actualiza si legatura de prev a nodului urmator degetului
    actual cu nodul nou creat.In caz contrat, nu vom aduce o alta modificare,
    deoarece urmatorul nod va fi NULL.
    */
    if (Banda_Magica->deget->next != NULL)
        Banda_Magica->deget->next->prev = nou;
    /*Actualizam legatura de next a degetului actual, deoarece nodul nou creat
    se afla la dreapta acestuia.
    */
    Banda_Magica->deget->next = nou;
    /*Punem in informatia de tip char a noului nod, caracterul char c
    primit ca parametru de functie 
    */
    nou->data = c;
    //Modificam pozitia degetului sa pointeze la noul nod creat
    Banda_Magica->deget = nou;
}

void Show_current(BandaMagica *Banda_Magica, FILE *output)
{ //Afisam in fisierul de output primit ca parametru, informatia din deget
    fprintf(output, "%c\n", Banda_Magica->deget->data);
}

void Show(BandaMagica *Banda_Magica, FILE *output)
{
    //Declaram un iterator pentru a parcurge banda
    BandaMagicaNod *p;
    for (p = Banda_Magica->sentinel->next; p != NULL; p = p->next)
        /*In cazul in care ne aflam pe pozitia la care pointeaza degetul
        facem afisarea caracteristica prin utilizarea celor doua linii
        intre care pozitionam informatia nodului la care pointeaza
        degetul |informatie|
        */
        if (p == Banda_Magica->deget)
            fprintf(output, "|%c|", p->data);
        else
            //Facem afisarea normala a fiecarui nod din banda,diferit de deget
            fprintf(output, "%c", p->data);
    /*Adaugam la finalul afisarii \n pentru a sarii la un nou rand pentru o
    noua afisare
    */
    fprintf(output, "\n");
}

void Initializare_Queue(Queue **q)
{ //Alocam dinamic memorie pentru coada si initializam cu NULL cei doi indici
    *q = (Queue *)malloc(sizeof(Queue));
    (*q)->front = NULL;
    (*q)->rear = NULL;
}

void AddQueue(Queue *q, char s[])
{ //Alocam dinamic memorie pentru noul nod pe care il vom adauga
    QueueNode *nou = (QueueNode *)malloc(sizeof(QueueNode));
    /*Fiind stringuri, am folosit functia strcpy pentru a copia parametrul 
    in informatia nodului nou alocat
    */
    strcpy(nou->data, s);
    //Fiind o coada,adaugam mereu la final asa ca legatura de next va fi NULL
    nou->next = NULL;
    //Verificam daca coada este goala
    if (q->rear == NULL)
    { /*In caz afirmativ,acutalizam ambii indici ai cozii pentru a pointa la
        noul nod
        */
        q->front = nou;
        q->rear = nou;
    }
    else
    { /*In caz contrar, acualizam legatura de next a ultimului nod din coada
        sa pointeze catre noul nod,iar apoi indicele de rear va fi actualizat
        si va pointa catre noul nod
        */
        q->rear->next = nou;
        q->rear = nou;
    }
}

void RemoveQueue(Queue *q)
{ //Declaram un pointer auxiliar pentru a putea elibera memoria primului nod
    QueueNode *p = q->front;
    //Actualizam indicele de front al cozii pentru a putea face eliberarea
    q->front = q->front->next;
    /*Verificam daca am ajuns la finalul cozii,caz in care actualizam si
    indicele de rear al cozii cu NULL
    */
    if (q->front == NULL)
        q->rear = NULL;
    //Eliberam memoria primului element din coada
    free(p);
}

void Initializare_Stack(Stack **s)
{
    /*Alocam dinamic memorie pentru stiva primita ca parametru si initializam
    indicele de top al stivei cu NULL
    */
    (*s) = (Stack *)malloc(sizeof(Stack));
    (*s)->top = NULL;
}

void PushStack(Stack *s, BandaMagicaNod *p)
{ //Alocam dinamic memorie pentru noul nod pe care il vom adauga
    StackNode *nou = (StackNode *)malloc(sizeof(StackNode));
    //Actualizam informatia noului nod cu pointerul p, primit ca parametru
    nou->data = p;
    /*Fiind o stiva, care este parcursa de la ultimul element catre primul
    legatura de next a noului nod va pointa catre acutalul top al stivei
    */
    nou->next = s->top;
    //Actualizam indicele de top al stivei cu noul nod adaugat
    s->top = nou;
}

void PopStack(Stack *s)
{ //Declaram un pointer auxiliar pentru a putea elibera memoria ultimului nod
    StackNode *p = s->top;
    //Actualizam indicele de top al stivei pentru a putea realiza eliberarea
    s->top = s->top->next;
    //Eliberam memoria ultimului element din stiva
    free(p);
}

void execute(Queue *q, Stack *UndoStack, Stack *RedoStack, BandaMagica *Banda_Magica, FILE *output)
{ /* Pentru lizibilitatea codului, am facut o conventie ca p=q->front prin
    declararea lui p, prin care am retinut primul nod din coada q primita ca
    parametru si asupra caruia vom efectua operatii
    */
    QueueNode *p = q->front;
    /* Pentru lizibilitatea codului, am facut o conventie ca deget sa fie
    pointerul la care pointeaza degetul in banda 
    */
    BandaMagicaNod *deget = Banda_Magica->deget;
    /*Prin urmatoarea secventa de cod vom determina ce comanda se afla pe
    prima pozitie in coada si care functie va trebui apelata pentru a indeplini
    corect cerinta
    */
    /*Prin aceasta comanda, verificam daca "WRITE" se fla in stringul de pe
    prima pozitie
    */
    if (strstr(p->data, "WRITE"))
    { /*In caz afirmativ, apelam functia de Write, oferind ca parametrii
        banda magica si penultimul caracter din stringul care se afla in
        coada, deoarece ultimul caracter este '\0'
        */
        Write_C(Banda_Magica, p->data[strlen(p->data) - 1]);
    }
    /*Prin aceasta comanda, verificam daca "MOVE_RIGHT_CHAR" se fla in stringul
     de pe prima pozitie.Vom face aceasta verificare inaintea celei de 
     "MOVE_RIGHT", deoarece este mai restrictiva,iar pentru "MOVE_RIGHT",
     s-ar fi incadrat ambele comenzi.
    */
    if (strstr(p->data, "MOVE_RIGHT_CHAR"))
    { /*In caz afirmativ, apelam functia de "Move_right_char", oferind ca
        parametrii banda magica si penultimul caracter din stringul care se
        afla in coada, deoarece ultimul caracter este '\0'
        */
        Move_right_char(Banda_Magica, p->data[strlen(p->data) - 1]);
    }
    /*Daca "MOVE_RIGHT_CHAR" nu a fost gasit, vom cauta in siguranta daca
    "MOVE_RIGHT" se afla in stringul de pe prima pozitie.
    */
    else if (strstr(p->data, "MOVE_RIGHT"))
    { /*In caz afirmativ, apelam functia de "Move_right", oferind ca
        parametru banda magica.
        */
        Move_right(Banda_Magica);
        //Adaugam pozitia initiala a degetului in stiva de UNDO
        PushStack(UndoStack, deget);
    }
    /*Prin aceasta comanda, verificam daca "MOVE_LEFT_CHAR" se fla in stringul
     de pe prima pozitie.Vom face aceasta verificare inaintea celei de 
     "MOVE_LEFT", deoarece este mai restrictiva,iar pentru "MOVE_LEFT",
     s-ar fi incadrat ambele comenzi.
    */
    if (strstr(p->data, "MOVE_LEFT_CHAR"))
    { /*In caz afirmativ, apelam functia de "Move_left_char", oferind ca
        parametrii banda magica si penultimul caracter din stringul care se
        afla in coada, deoarece ultimul caracter este '\0'
        */
        Move_left_char(Banda_Magica, p->data[strlen(p->data) - 1], output);
    }
    /*Daca "MOVE_LEFT_CHAR" nu a fost gasit, vom cauta in siguranta daca
    "MOVE_LEFT" se afla in stringul de pe prima pozitie.
    */
    else if (strstr(p->data, "MOVE_LEFT"))
    { /*In caz afirmativ, apelam functia de "Move_left", oferind ca
        parametru banda magica.
        */
        Move_left(Banda_Magica);
        /*Verificam daca functia s-a realizat cu succes prin verificarea daca
        pozitia noua a degetului este diferita de cea initiala
        */
        if (deget != Banda_Magica->deget)
            /*In caz afirmativ adaugam pozitia initiala a degetului in stiva de
             UNDO
            */
            PushStack(UndoStack, deget);
    }
    /*Prin aceasta comanda, verificam daca "INSERT_LEFT" se fla in stringul de
    pe prima pozitie.
    */
    if (strstr(p->data, "INSERT_LEFT"))
        /*In caz afirmativ, apelam functia de "INSERT_LEFT", oferind ca
        parametrii banda magica si penultimul caracter din stringul care se
        afla in coada, deoarece ultimul caracter este '\0'
        */
        Insert_left(Banda_Magica, p->data[strlen(p->data) - 1], output);
    /*Prin aceasta comanda, verificam daca "INSERT_RIGHT" se fla in stringul de
    pe prima pozitie.
    */
    if (strstr(p->data, "INSERT_RIGHT"))
        /*In caz afirmativ, apelam functia de "INSERT_RIGHT", oferind ca
        parametrii banda magica si penultimul caracter din stringul care se
        afla in coada, deoarece ultimul caracter este '\0'
        */
        Insert_right(Banda_Magica, p->data[strlen(p->data) - 1]);
    /*Eliminam primul nod din coada,pentru ca a fost interpretat si dealocam
    memoria acestuia
    */
    RemoveQueue(q);
}

void rezolvare(BandaMagica *Banda_Magica, Queue *q, Stack *UndoStack, Stack *RedoStack, FILE *in, FILE *out)
{ /*Initializam n-ul care va fi numarul de operatii din fisier ce se afla pe
    primul rand din fisier, iar i, un simplu iterator printr-un for
    */
    int n, i;
    //Citim din fisierul de input numarul de operratii
    fscanf(in, "%d\n", &n);
    /*Declararm un s static de 18 caractere, deoarece cea mai lunga comanda,
    respectiv "MOVE_LEFT_CHAR X" are 17 caractere
    */
    char s[18];
    /*Citim fiecare comanda din fisier cu acest for, si facem operatiile
    necesare
    */
    for (i = 0; i < n; i++)
    { //Citim comanda din fisier
        fgets(s, 101, in);
        /*La final, comanda fgets pune \n inainte de \0, asa ca trebuie sa
        mutam \0 cu o pozitie mai la stanga
        */
        s[strlen(s) - 1] = '\0';
        /*Verificam daca comanda citita este unda de update, iar in caz
        afirmativ o introducem in coada
        */
        if (strstr(s, "MOVE") != NULL || strstr(s, "WRITE") != NULL || strstr(s, "INSERT") != NULL)
            //Adaugam in coada q,comanda s
            AddQueue(q, s);
        /*Verificam daca comanda citita este "EXECUTE", caz incare se apeleaza
        funcita execute
        */
        if (strstr(s, "EXECUTE") != NULL)
            //Apelam funcita execute
            execute(q, UndoStack, RedoStack, Banda_Magica, out);
        //Verificam daca comanda citita este "UNDO"
        if (strstr(s, "UNDO") != NULL)
        { //Adaugam in stiva de REDO, pointerul extras din stiva de UNDO
            PushStack(RedoStack, Banda_Magica->deget);
            /*Modifica pozitia degetului sa pointeze la adresa pointerului
            extras din stiva de UNDO
            */
            Banda_Magica->deget = UndoStack->top->data;
            /*Se elimina pointerul extras din stiva de UNDO si se elibereaza 
            memoria aferenta
            */
            PopStack(UndoStack);
        }
        if (strstr(s, "REDO") != NULL)
        { //Adaugam in stiva de UNDO, pointerul extras din stiva de REDO
            PushStack(UndoStack, Banda_Magica->deget);
            /*Modifica pozitia degetului sa pointeze la adresa pointerului
            extras din stiva de REDO
            */
            Banda_Magica->deget = RedoStack->top->data;
            /*Se elimina pointerul extras din stiva de REDO si se elibereaza
            memoria aferenta
            */
            PopStack(RedoStack);
        }
        /*Verificam daca comanda citita este "SHOW_CURRENT", deoarece este mai
        restrictiva decat cea de "SHOW", in cazul careia se regasesc ambele
        comenzi
        */
        if (strstr(s, "SHOW_CURRENT") != NULL)
            /*In caz afirmativ, apelam functia "Show_current", oferind ca
            parametrii Banda_Magica si fisierul de output
            */
            Show_current(Banda_Magica, out);
        else
            //In caz contrar,verificam daca comanda citia este "SHOW"
            if (strstr(s, "SHOW") != NULL)
            /*In caz afirmativ, apelam functia "Show", oferind ca
            parametrii Banda_Magica si fisierul de output
            */
            Show(Banda_Magica, out);
    }
}

void free_Stack(Stack *s)
{ //Verificam ca stiva s sa nu fie goala
    while (s->top != NULL)
        //Eliberam de fiecare data memoria ultimului element din stiva
        PopStack(s);
    //Eliberam memoria alocata stivei in sine
    free(s);
}

void free_Queue(Queue *q)
{ //Verificam sa nu fie goala coada q
    while (q->rear != NULL)
        //Eliberam de fiecare data memoria primului element din coada
        RemoveQueue(q);
    //Eliberam memoria alocata cozii in sine
    free(q);
}

void free_BandaMagica(BandaMagica *Banda_Magica)
{ //Verificam ca Banda_Magica sa nu fie goala
    while (Banda_Magica->sentinel->next != NULL)
    { //Declaram un pointer auxiliar pentru a putea elibera memoria primului nod
        BandaMagicaNod *p = Banda_Magica->sentinel->next;
        //Actualizam legatura de next a santinelei pentru a putea realiza eliberarea memoriei
        Banda_Magica->sentinel->next = Banda_Magica->sentinel->next->next;
        //Eliberam memoria elementului din Banda_Magica
        free(p);
    }
    //Eliberam memoria destinata santinelei
    free(Banda_Magica->sentinel);
    //Eliberam memoria Bandei_Magice propriu-zisa
    free(Banda_Magica);
}
