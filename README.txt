Galie Ana Maria 322CD
IOCLA Tema (1)

_._._._[[Functii folosite (In ordinea din fisier)]]_._._._

(int) = (i)
(void) = (v)
(char*) = (c*)
(+) = Exceptii/In plus

[tema1.c]: 
        -(i) check_file_existence 
        -(i) check_dir_existence 
        -(v) touch 
        -(v) mkdir 
        -(v) ls 
        -(v) rm 
        -(v) remove_files
        -(v) rmdir
        -(v) cd
        -(v) recursive_path
        -(c*) pwd
        -(v) remove_dir
        -(v) stop
        -(v) printf_files
        -(v) tree
        -(v) mv_file
        -(v) mv_dir
        -(v) mv
        -main

_._._._._._._._._._[[Structuri]]_._._._._._._._._._._   

[Liste simplu inlantuite]: 
Implementat cu: typedef struct Dir + typedef struct File

typedef struct File_______:
    char *name:  numele fisierului
    struct Dir* parent: (directorul) in care se afla (fisierul)
    struct File* next: link spre urmatorul fisier din lista de fisiere
                        din care face parte "node-ul"

typedef struct Dir________:
    char *name: numele directorului
    struct Dir* parent: (directorul) in care se afla (directorul)
    struct File* head_children_files: capul listei de (fisiere) al directorului
    struct Dir* head_children_dirs: capul listei de (directoare) al directorului
    struct Dir* next: link spre urmatorul director din lista de directoare
                        din care face parte "node-ul"

_._._._._._._._._._[[Functii]]_._._._._._._._._._._._

[main]________________________:

    Description: 
                Se aloca memorie pentru folderul/directorul home de baza.
                Do-While Loop: alocam memorie pentru comanda si citim string-ul
                de la stdin cu scanf.

                Se verifica daca string-ul citit este valid (corespunde cu
                numele vreunei comenzi din sistem).
                In caz contrar string-ul va fi ignorat si dealocat cu free().

                In functie de comanda valida:
                > Se dealoca memoria comenzii
                > Daca comanda are nevoie de argumente (unu/doua) pentru a functiona 
                acestora li se va aloca memorie si se citesc de la stdin
                > Pentru fiecare comanda se va apela functia cu numele aferent
                > Se dealoca numele argumentului daca este cazul
                > (+) (pwd): se afiseaza path-ul
                > (+) (stop): Iese la sfarsit din aplicatie (break)



[check_file_existence]________:

    Parameter(s): (Dir* current_folder, char* name)
                current_folder: directorul curent in care se lucreaza
                name: numele (fisierului) care este cautat in director

    Return value: error_file

    Description: 
                Cauta prin directorul curent existenta numelui fisierului pe care 
                se doreste sa fie creat.
                In caz contrar, (error_file) va ramane '0'.

[check_dir_existence]_________:

    Parameter(s): (Dir* current_folder, char* name)
                current_folder: directorul curent in care se lucreaza
                name: numele (directorului) care este cautat in director

    Return value: error_dir

    Description: 
                Cauta prin directorul curent existenta numelui directorul pe care 
                se doreste sa fie creat.
                In caz contrar, (error_dir) va ramane '0'.

[touch]_______________________:

    Parameter(s): (Dir* parent, char* name)
                parent: directorul curent in care se doreste crearea (fisierului)
                name: numele fisierului care se doreste sa fie creat

    Description: 
                Se apeleaza (check_dir_existence) si (check_dir_existence).
                Lipsa numelui fisierului in lista de directoare/fisiere duce la
                afisarea mesajului "File/Directory already exists\n".

                Daca fisierul nu a fost gasit in nici o lista:
                > Se aloca memorie pentru noul node/fisier
                > Se atribuie numele dorit la creare, directorul curent ca parent, iar 
                link-ul (next) va fi setat NULL
                > Daca capul listei de fisiere este NULL (lista este empty)
                noul fisier devine cap. Altfel, este pus la sfarsitul listei
                

[mkdir]_______________________:

    Parameter(s): (Dir* parent, char* name)
                parent: directorul curent in care se doreste crearea (directorului)
                name: numele directorului care se doreste sa fie creat
    
    Description: 
                Se apeleaza (check_dir_existence) si (check_dir_existence).
                Lipsa numelui fisierului in lista de directoare/fisiere duce la
                afisarea mesajului "File/Directory already exists\n".

                Daca directorul nu a fost gasit in nici o lista:
                > Se aloca memorie pentru noul node/director
                > Se atribuie numele dorit la creare, directorul curent ca parent, iar 
                link-ul (next), (head_children_dirs) si (head_children_files)
                proprii vor fi setate cu NULL
                > Daca capul listei de directoare este NULL (lista este empty)
                noul director devine cap. Altfel, este pus la sfarsitul listei

[ls]__________________________:

    Parameter(s): (Dir* parent)
                parent: directorul curent caruia i se doreste afisat continutul
    
    Description: 
                Daca lista de fisiere/directoare nu este empty se va trece prin aceasta
                (in ordinea de creare) si pentru fiecare node se va afisa numele.

[rm]__________________________:

    Parameter(s): (Dir* parent, char* name)
                parent: directorul din care se doreste sters un fisier
                name: numele fisierul care se doreste sters
    
    Description: 
                Daca lista de fisiere nu este empty avansam.
                In caz contrar, se afiseaza "Could not find the file".

                Cautam prin lista existenta folosindu-ne de (2) pointeri.
                temp_file: va incepe cu o pozitie dupa capul listei
                prev: va fi tot timpul cu o pozitie in lista in spate fata de (temp_file)  

                Se analizeaza cazurile in care fisierul:
                (1) Este singurul din lista si nu ce cautam 
                (2) Este primul si singur in lista
                (3) Este in interiorul/la sfarsitul listei
  
                Pentru (1) in mod specific iesim direct din loop +
                Se afiseaza "Could not find the file"

[remove_files]________________:

    Parameter(s): (Dir* current_folder)
                current_folder: directorul curent din care vrem fisierele sterse
    
    Description: 
                Se aloca memorie pentru un array de string-uri presupunand ca 
                utilizatorul nu va crea mai mult de (MAX_INPUT_LINE_SIZE) (300) 
                fisiere in directorul home.
                Se parcurge lista de fisiere pentru directorul curent dat, 
                alocam si stocam numele lor in array-ul (name_files).
                Apelam (rm) pentru fiecare nume de fisier din (name_files).
                Dealocam memoria numelelor si array-ul post folosire.

[rmdir]_______________________:

    Parameter(s): (Dir* parent, char* name)
                parent: directorul din care se doreste sters un director
                name: numele directorului care se doreste sters
    
    Description: 
                Functie recursiva care se apeleaza pe ea insasi cu subdirectoarele
                din directorul curent.
                La fiecare apel:
                > Daca exista cap de lista de directoare se intra in while loop.
                > Daca lista este empty sau directorul cautat nu a fost gasit in loop
                se afiseaza "Could not find the dir".
                > In momentul gasirii, daca este primul capul listei avanseaza,
                altfel se refac legaturile folosindu-ne de un node prev ajutator.
                
                Daca directorul care trebuie sters are subdirectoare se reapeleaza 
                functia recursiv pentru acestea.
                Recursivitatea ajunge la un capat cand nu mai sunt subdirectoare intr-un
                director curent. 
                Pentru toate directoarele:
                > Li se va sterge fisierele interioare
                > Dealoca memoria numelui si a node-ului reprezentant in sine
                
[cd]__________________________:

    Parameter(s): (Dir** target, char *name)
                target: directorul curent care trebuie schimbat
                name: numele directorul in care vrem sa ajungem
    
    Description:
                Argumentul dat in name este:
                > "..": Schimbam directorul curent in parintele sau (Daca nu este NULL
                        /Nu suntem in directorul home deja)
                > Un string oarecare: Cautam daca directorul exista in lista 
                        de directoare. In momentul gasirii se face schimbarea si
                        se iese din functie.
                        Altfel, se afiseaza "No directories found!"

[recursive_path]______________:

    Parameter(s): (Dir* target, char *path)
                target: directorul curent caruia ii vrem path-ul pana la home
                path: string-ul in care vom crea path-ul

    Description: 
                Functie recursiva care se reapeleaza pe sine cu parintele directorului
                primit si path-ul.
                Recursivitatea se intoarce/ajunge la capatul dorit cand directorul
                nu mai are parinte (directorul home).
                Path-ul se contruieste prin concatenare recursiva adaugandu-se
                la fiecare intoarcere "/" + "numele directorului" incepand cu "/home".

[pwd]_________________________:

    Parameter(s): (Dir* target)
                target: directorul curent caruia ii vrem path-ul pana la home

    Return value: path (pointer la string-ul in care a fost creat path-ul)

    Description: Alocam memorie presupunand ca marimea maxima realistica ar fi de 5 ori
                mai mare decat limita unei linii de comanda (MAX_INPUT_LINE_SIZE).
                Apelam functia recursiva (recursive_path).

[remove_dir]__________________:

    Parameter(s): (Dir* current_folder)
                current_folder: directorul curent din home pe care-l dorim sters
                            impreuna cu continutul sau
    
    Description: 
                Se aloca memorie pentru un array de string-uri presupunand ca 
                utilizatorul nu va crea mai mult de (MAX_INPUT_LINE_SIZE) 
                (300) directoare in directorul home.
                Se parcurge lista de directoare pentru home, alocam si stocam numele lor
                in array-ul (name_dirs).
                Apelam (rmdir) pentru fiecare nume de director din (name_dirs).
                Dealocam memoria numelelor si array-ul post folosire.


[stop]________________________:

    Parameter(s): (Dir* target)
                target: directorul curent in care ne aflam la solicitarea opririi
                        aplicatiei
    
    Description:
                Daca la apelarea functiei nu suntem in directorul home se va
                apela cd pana cand vom regresa in ierarhie in home.
                Se apeleaza pentru directorul home (remove_dir) si (remove_files) si 
                se dealoca memoria pentru directorul home iesind din functie si 
                ulterior din loop-ul aplicatiei.

[printf_files]________________:

    Parameter(s): (File* temp_file, int level)
                temp_file: capul listei de fisiere al directorului curent
                level: nivelul in care ne aflam in ierarhia de fisiere si directoare
    
    Description:
                Parcurgem lista de fisiere.
                Fiecare nume de fisier va fi afisat in format de (4 spatii libere)
                per nivel de iararhie + numele sau.

[tree]________________________:

    Parameter(s): (Dir* target, int level)
                target: directorul curent din care dorim sa se afiseze ierarhia
                        pe nivele
                level: nivelul de incepere al ierarhiei (default '0')
    
    Description: 
                Functie recursiva care trece prin toate fisiere si directoarele 
                directorului curent si le afiseaza cu un format de (4 spatii libere)
                per nivel de ierarhie + numele fisierului/directorului
                Functia se reapeleaza pe ea insasi cu subdirectoarele directorului
                curent si level-ul de ierarhie (+1).

                Cazurile in care recursivitatea se intoarce/ajunge la un capat 
                dorit in rulare sunt:
                > Directorul nu are nici fisiere nicin directoare
                > Directorul are doar fisiere 

                Fisierele din ierarhie se afiseaza apelandu-se (printf_files)
                (In ultimul caz de intoarcere si in cazul in care directorul current 
                are si fisiere pe langa directoare in ierarhia sa)

[mv_file][mv_dir]_____________:

    Note: Ambele functii lucreaza dupa aceleasi principii.
        Diferenta principala este management-ul de tipuri diferite de date.

    Parameter(s): (Dir* current_folder, char *oldname, char *newname)
                current_folder: directorul curent in care dorim schimbarea
                oldname: numele vechi al fisierului/directorului
                newname: numele nou in care vrem sa schimbam oldname

    Description:
                Se folosesc (3) pointeri: prev, temp_file/temp_dir, end;
                temp_file/temp_dir: va incepe cu o pozitie dupa capul listei
                prev: va fi tot timpul cu o pozitie in lista in spate fata de (temp_file/dir)
                end: va fi adus pe ultimul node/fisier/director din lista de fisiere/directoare

                Se analizeaza cazurile in care fisierul/directorul:
                (1) Este singurul din lista
                (2) Este primul dintr-o lista 
                (3) Este in interiorul/la sfarsitul listei  
                
                In toate cazurile memoria alocate anterior pentru nume este dealocata
                si realocata cu (newname).
                Pentru situatiile (1) si (2) se refac link-urile intre noduri 
                (si se muta capul listei dupa caz) si se plaseaza fisierul/directorul 
                cu numele schimbat la sfarsitul listei.

[mv]__________________________:

    Parameter(s): (Dir* parent, char *oldname, char *newname)
                parent: directorul curent in care dorim schimbarea
                oldname: numele vechi al fisierului/directorului
                newname: numele nou in care vrem sa schimbam oldname
    
    Description:
                Apelam functiile (check_file_existence) si (check_dir_existence)
                de doua ori (cu (oldname) si (newname))

                Se analizeaza cazurile:
                (1) Nu exista nici un fisier/director cu numele vechi in directorul curent
                se afiseaza "File/Director not found"
                (2) Cel putin un fisier sau director exista deja cu numele dorit pentru
                schimbare se va afisa "File/Director already exists"

                In caz contrar celor anterioare se apeleaza (mv_file) sau (mv_dir).
                (functiile (touch) si (mkdir) ne asigura ca un fisier si un director nu
                vor avea acelasi nume in acelasi timp) 


