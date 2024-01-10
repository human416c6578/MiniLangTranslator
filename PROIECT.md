##1. Enunt

Sa se implementeze in limbajul C/C++ un program care sa realizeze operatia Traducere in C pentru un limbaj de programare minimalist ce poate contine urmatoarele elemente:

0 – Comentarii: `# Acesta e un comentariu (mono-linie). Tot ce e de la simbolul '#' pana la finalul liniei e ignorat.`

1 – Definire de variabila: `var nume_variabila`

2 – Definire de vector: `var nume_vector[numar_elemente]`

3 – Atribuire: `Variabila = Expresie`, unde:
   - Variabila poate fi `nume_variabila` sau `nume_vector[index_element]`
   - Expresie e orice expresie aritmetica valida ce poate contine operatorii aritmetici: +, -, (, ) si drept operanzi poate contine variabile (`nume_variabila` sau `nume_vector[index_element]`) sau valori numerice (reprezentand numere intregi pozitive scrise in baza 10).

4 – Decizie: 
   ```
   if Conditie
       Instructiuni
   else
       Instructiuni
   end
   ```
   unde:
   - Conditie este o expresie ce poate sa contina variabile sau valori numerice si operatorii relationali > < = si operatorul logic | (sau),
   - Instructiuni e orice succesiune (inclusiv vida) de instructiuni (dintre acestea pe care le recunoaste limbajul).

5 – Repetitie: 
   ```
   while Conditie
       Instructiuni
   end
   ```

6 – Afisare: `print Variabila` sau: `print ”orice sir de caractere inclus intre ghilimele, inclusiv \n pentru 'linie noua'...”`

7 – Citire: `read Variabila`

Variabilele din program (atat cele scalare, cat si elementele din vectori) vor fi de tipul intreg. Toti operatorii vor avea asociativitate la stanga, iar operatorii relationali vor avea precedenta mai mare decat operatorul logic.

##2. Prezentare generala

Programul este compus din 3 clase : Lexer, AST si Translator

###Lexer
Clasa `Lexer` are două atribute: o variabilă de tip string numită `script_`, în care este salvat textul ce urmează a fi parsat, și o altă variabilă de tip int numită `pos_`, în care se păstrează poziția curentă în text.

Clasa `Lexer` se ocupă de transformarea input-ului (fișierul text) într-un vector de `Tokens`. 
Un `Token` este reprezentat de o structură care are următoarele atribute: `type`, `value` și `line`.

1. `type`
   - `type` este de tipul `TokenType`.
   - `TokenType` este un enum care cuprinde toate tipurile de tokens, de exemplu: `VAR`, `IDENTIFIER`, `NUMBER`, `IF`, `WHILE`, etc.

2. `value`
   - `value` este de tipul string pentru `Tokens` care au o anumită valoare, de exemplu: `STRING`, `NUMBER`.

3. `line`
   - `line` este de tipul int și reprezintă linia din fișier pe care se regăsește token-ul.

Constructorul clasei `Lexer` primește ca parametru un string, care mai apoi este parcurs pentru a crea vectorul de tip `Token`.

Funcția `parse` din clasa `Lexer` se ocupă de parcurgerea textului și generarea vectorului de `Tokens`. Iată cum funcționează această funcție:

1. **Inițializare:** Se inițializează un vector gol de `Token`-uri și variabila `line` care reprezintă numărul liniei curente.

2. **Parcurgere:** Folosind o buclă `while`, funcția parcurge fiecare caracter din text până la sfârșitul acestuia (`pos_ < script_.size()`).

3. **Verificare linie nouă (`'\n'`):** Dacă caracterul curent este o linie nouă (`'\n'`), se incrementează variabila `line`.

4. **Comentarii (`'#'`):** Dacă caracterul curent este `'#'`, se apelează funcția `skipLine` pentru a sări peste întreaga linie.

5. **Șiruri de caractere (`'"'`):** Dacă caracterul curent este `'"'`, se apelează funcția `parseString` pentru a extrage și adăuga un `Token` de tip `STRING` în vectorul `tokens`.

6. **Spații albe (`isspace`):** Dacă caracterul curent este un spațiu alb, acesta este ignorat prin incrementarea variabilei `pos_`.

7. **Numere (`isdigit`):** Dacă caracterul curent este un număr, se apelează funcția `parseNumber` pentru a extrage și adăuga un `Token` de tip `NUMBER` în vectorul `tokens`.

8. **Cuvinte cheie sau identificatori (`isalpha`):** Dacă caracterul curent este o literă, se apelează funcția `parseKeywordOrIdentifier` pentru a extrage și adăuga un `Token` corespunzător în vectorul `tokens`.

9. **Caractere speciale (`switch`):** Dacă caracterul curent este altceva decât cele menționate mai sus, se folosește o structură `switch` pentru a gestiona caracterele speciale precum +, -, *, /, <, >, ,, (, ), [, ], &, |, =. Se apelează funcții specifice pentru anumite simboluri și se adaugă `Token`-urile corespunzătoare în vectorul `tokens`.

10. **EOF Token:** La final, se adaugă un `Token` de tip `EOF_TOKEN` pentru a marca sfârșitul fluxului de `Tokens`.

11. **Returnare:** Se returnează vectorul complet de `Tokens`.

Această funcție parcurge și analizează fiecare caracter din text, generând un vector structurat de `Tokens` care pot fi folosite pentru analiza sintactică și execuția programului asociat.

În general, pentru a determina dacă o porțiune de text reprezintă un atom lexical, se examinează prima literă a acesteia pentru a identifica categoria posibilă. Ulterior, textul este parcurs până când există doar o variantă posibilă, funcțiile precum parseNumber sau parseString aplicând această logică.

De exemplu, în cazul funcției **parseKeywordOrIdentifier**, unde atomul lexical începe cu o literă, se inițiază o buclă while care parcurge textul de la poziția curentă, generând un șir de caractere (value) până când următorul caracter nu mai este o literă. Există și verificări pentru simbolurile `_` și `-`, care pot fi prezente în numele unei variabile. Dacă ultimul caracter este `-`, atomul lexical nu poate exista, deoarece nu există niciun tip de atom lexical care să corespundă acestui format.

Apoi, printr-un set de instrucțiuni if-else se verifică dacă șirul value corespunde cu tipurile de atomi lexicali precum var, if, while, end, print, read. În cazul în care nu corespunde cu niciunul dintre acestea, tipul acestui atom lexical este considerat un identifier, adică un nume de variabilă, având în vedere că s-au verificat respectarea regulilor pentru numele unei variabile.

În final, funcția returnează atomul lexical generat, iar poziția este actualizată pentru a reflecta progresul în parcurgerea textului.

```cpp
Token Lexer::parseKeywordOrIdentifier(int line)
{
    std::string value;
    while (isalnum(script_[pos_]) || script_[pos_] == '_' || script_[pos_] == '-')
    {
        value += script_[pos_++];
    }

    if (value.back() == '-')
    {
        std::cerr << "[" << line << "]" << "Unexpected character ( - )";
        return {};
    }

    // Check if the identifier is a keyword
    if (value == "var")
        return {VAR, value, line};
    else if (value == "if")
        return {IF, value, line};
    else if (value == "while")
        return {WHILE, value, line};
    else if (value == "print")
        return {PRINT, value, line};
    else if (value == "read")
        return {READ, value, line};
    else if (value == "else")
        return {ELSE, value, line};
    else if (value == "end")
        return {END, value, line};
    else
        return {NAME, value, line}; // Assume it's an identifier if not a keyword
}
```

### AST (Arborele de Sintaxă Abstractă)

Clasa `AST` are două atribute principale: o variabilă de tip string numită `tokens_`, unde este stocat vectorul de atomi lexicali ce va fi parsat, și o altă variabilă de tip int numită `pos_`, responsabilă cu urmărirea poziției curente în vector.

Clasa `AST` se ocupă de transformarea input-ului (vectorul de atomi lexicali) într-un Arbore de Sintaxă Abstractă (AST), construit din structuri de tip `ASTNode`.

Un `ASTNode` este reprezentat printr-o structură ce conține următoarele atribute: `type`, `value`, și `children`.

1. `type`
   - `type` este de tipul `ASTNodeType`.
   - `ASTNodeType` este un enum care include toate tipurile de noduri specifice Arborelui de Sintaxă Abstractă. De exemplu: `VARIABLE_DECLARATION` (Declarație de Variabilă), `ASSIGNMENT` (Atribuire), `IF_STATEMENT` (Instrucțiune IF), `WHILE_LOOP` (Buclă WHILE), `PLUS_OPERATION` (Operație de Adunare), etc.

   ASTNodeType furnizează o modalitate eficientă de etichetare și distingere a diferitelor tipuri de noduri într-un AST, ușurând astfel analiza și interpretarea programelor. Fiecare tip de nod corespunde unei construcții sintactice specifice în limbajul de programare respectiv.

2. `value`
   - `value` este de tipul string pentru nodurile care au o valoare asociată.

3. `children`
   - `children` este de tipul vector de `ASTNode` și reprezintă copiii nodului curent.

#### Constructor și Funcția Parse

```cpp
AST(const std::vector<Token> &tokens): tokens_(tokens), pos_(0) {}

void AST::parse(ASTNode &Program)
{
    while (pos_ < tokens_.size())
    {
        switch (current().type)
        {
        case TokenType::VAR:
            Program.children.push_back(parseDeclaration());
            break;
        case TokenType::NAME:
            Program.children.push_back(parseAssignment());
            break;
        case TokenType::PRINT:
            Program.children.push_back(parsePrint());
            break;
        case TokenType::READ:
            Program.children.push_back(parseRead());
            break;
        case TokenType::WHILE:
            Program.children.push_back(parseWhile());
            break;
        case TokenType::IF:
            Program.children.push_back(parseIf());
            break;
        default:
            consume();
            break;
        }
    }
}
```

**Constructorul:**
- Constructorul clasei `AST` primește un vector de atomi lexicali `tokens` și îl atribuie variabilei membre `tokens_`. De asemenea, inițializează `pos_` cu 0 pentru a marca poziția curentă în vector.

**Funcția Parse:**
- Funcția `parse` are ca responsabilitate construirea Arborele de Sintaxă Abstractă (AST). În timpul execuției, funcția parcurge vectorul de atomi lexicali, analizându-i pentru a identifica tipul de nod potențial.
  - Se examinează atomii lexicali pentru a identifica tipul de nod posibil.
  - Vectorul este parcurs în continuare pentru a identifica exact acel tip de nod.
  - La finalul acestui proces, funcția returnează un obiect de tip `ASTNode`, care reprezintă nodul construit pe baza atomilor lexicali analizați.

- Vom urmări un exemplu specific când funcția identifică un atom lexical de tip `VAR` și apelează funcția `parseDeclaration` pentru a continua procesul de analiză și construcție a nodului corespunzător.

#####Funcția parseDeclaration

Funcția `parseDeclaration` are rolul de a analiza și structura unui token care declară o variabilă (`var`). Iată cum funcționează:

1. **Crearea Nodului Variabilei (`varNode`):**
   - Se creează un obiect `ASTNode` numit `varNode`.
   - Se atribuie tipul `VARIABLE_DECLARATION` noului nod.

2. **Trecerea de Atomul Lexical 'var':**
   - Se trece la următorul atom lexical din vector prin apelul funcției `consume()`. Acest pas marchează că am trecut de token-ul 'var'.

3. **Verificarea Tipului Atomului Lexical Curent (`current().type`):**
   - Se verifică dacă atomul lexical curent este de tip `NAME` (identificator).
     - Dacă este `NAME`, se continuă cu analiza identificatorului prin apelul funcției `parseIdentifier()` și rezultatul este adăugat la lista de copii a nodului `varNode`.
     - Se verifică apoi dacă următorul atom lexical este de tip `EQUALS` (operator de atribuire).

4. **Verificarea Operatorului de Atribuire (`TokenType::EQUALS`):**
   - Dacă atomul lexical curent este de tip `EQUALS`, atunci se considera ca in nodul `VARIABLE_DECLARATION` se face si o operatie de `atribuire`, astfel se trece peste acest token (`EQUALS`) și se adaugă în lista de copii a nodului `varNode` rezultatul apelului funcției `parseExpression()`.
   - Funcția `parseExpression()` analizează și construiește un nod pentru expresii aritmetice sau logice asociate variabilei.

5. **Tratarea Situațiilor Neașteptate:**
   - În caz contrar, dacă atomul lexical nu este de tip `NAME`, se afișează o eroare prin intermediul `std::cerr`, deoarece dupa un atom de tip `VAR` ( declaratie de variabila ) intotdeauna se afla un atom de tip `NAME` ( identifier ).

6. **Returnarea Nodului Variabilei (`varNode`):**
   - Se returnează nodul `varNode`, care conține informațiile despre declarația variabilei, inclusiv numele și, dacă există, expresia asociată.

```cpp
ASTNode AST::parseDeclaration()
{
    ASTNode varNode;
    varNode.type = ASTNodeType::VARIABLE_DECLARATION;
    consume(); // Trecem de atomul lexical 'var'

    if (current().type == TokenType::NAME)
    {
        // Analizăm numele variabilei
        varNode.children.push_back(parseIdentifier());
        // Verificăm pentru specificatorul de atribuire
        if (current().type == TokenType::EQUALS)
        {
            consume(); // Trecem de atomul lexical 'equals'
            varNode.children.push_back(parseExpression());
        }
    }
    else
    {
        std::cerr << "Eroare, atom lexical invalid!";
    }

    return varNode;
}
```

### Translator

Clasa `Translator` are rolul de a traduce un Arbore de Sintaxă Abstractă (AST) în cod sursă în limbajul de programare C++. Aceasta facilitează conversia structurilor sintactice ale unui program într-un format care poate fi înțeles și executat de un compilator C++. Principalele funcționalități ale clasei includ:

#### Funcția `ast_to_cpp` în cadrul clasei Translator

Funcția `ast_to_cpp` reprezintă inima clasei `Translator`, asigurând traducerea adecvată a unui nod din Arborele de Sintaxă Abstractă (AST) în cod sursă C++. Iată cum funcționează și cum este gestionată traducerea pentru diverse tipuri de noduri:

1. **Structura Funcției:**
   - Funcția `ast_to_cpp` este definită static în clasa `Translator` și primește două argumente: `node`, care reprezintă nodul din AST care trebuie tradus, și `depth`, care indică nivelul de înglobare pentru formatarea corectă a codului.

2. **Abordarea Recursivă:**
   - Funcția utilizează o abordare recursivă pentru a explora toate ramurile arborelui, traducând fiecare nod și gestionându-și copiii.
   - Pentru fiecare tip de nod, funcția identifică tipul și aplică logica specifică pentru traducerea acelui tip.

3. **Utilizarea Lambda pentru Adăugarea de Indentații:**
   - Funcția utilizează o lambda `add_indentation` pentru a adăuga spații albe pentru formatarea corectă a codului în funcție de nivelul de înglobare (`depth`).
   - Aceasta furnizează un mod elegant de a introduce spații și linii goale pentru evidențierea structurii blocurilor de cod.

4. **Instrucțiuni Switch Pentru Tipurile de Noduri:**
   - Funcția utilizează o instrucțiune `switch` pentru a identifica tipul nodului (`node.type`) și a apela funcția corespunzătoare pentru traducerea acelui tip de nod.
   - Fiecare caz în `switch` corespunde unui tip specific de nod din AST.

5. **Traducerea Diferitelor Tipuri de Noduri:**
   - Funcția conține cazuri pentru diferite tipuri de noduri, cum ar fi `VARIABLE_DECLARATION`, `ASSIGNMENT`, `IF_STATEMENT`, `STRING_LITERAL`, `NUMBER_LITERAL`, etc.
   - În cazul nodurilor care au copii (de exemplu, blocuri de cod, expresii aritmetice), funcția apelată recursiv pentru a traduce și acei copii.

6. **Gestionarea Profunzimii Blocurilor de Cod:**
   - Pentru instrucțiunile de control de flux precum `IF_STATEMENT` și `WHILE_LOOP`, funcția asigură o formatare corectă a blocurilor de cod, adăugând spații în funcție de nivelul de înglobare (`depth`).

7. **Returnarea Codului C++ Generat:**
   - Funcția returnează un șir de caractere care reprezintă codul C++ generat pentru nodul dat din AST.
   - La nivelul cel mai înalt, această funcție poate fi apelată pentru a traduce întregul program.

Iată un exemplu simplificat al modului în care funcția `ast_to_cpp` ar putea traduce un nod de tip `ASSIGNMENT`:

```cpp
case ASTNodeType::ASSIGNMENT:
{
    std::string left_operand = ast_to_cpp(node.children[0], depth);
    std::string right_operand = ast_to_cpp(node.children[1], depth);
    return add_indentation() + left_operand + " = " + right_operand + ";";
}
```

În acest exemplu, funcția traduce o instrucțiune de atribuire, aplicând recursiv traducerea pentru stânga și dreapta operatorilor, iar rezultatul este o linie de cod C++ corect indentată.


### Main

Funcția `main` din programul C++ are rolul de a coordona și controla fluxul principal al aplicației. În esență, aceasta îndeplinește următoarele funcționalități la modul general:

1. **Verificarea Argumentelor de Linie de Comandă:**
   - Se verifică dacă programul a fost apelat cu un număr suficient de argumente de linie de comandă (cel puțin un nume de fișier de intrare).
   - Se afișează mesaje de utilizare în caz de parametri insuficienți.

2. **Procesarea Flag-urilor:**
   - Se analizează argumentele de linie de comandă pentru a determina dacă trebuie să se afișeze și vectorul de atomi lexicali și/sau arborele de sintaxă abstractă.
   - Flag-urile `--tokens` și `--ast` activează afișarea corespunzătoare.

3. **Deschiderea și Citirea Fișierului de Intrare:**
   - Se deschide fișierul specificat de pe linia de comandă și se citește conținutul acestuia într-un șir de caractere (`script`).

4. **Instantierea Obiectelor și Parsarea:**
   - Se creează un obiect de tip `Lexer` și se apelează funcția `parse` pentru a transforma șirul de caractere într-un vector de atomi lexicali (`tokens`).
   - Se creează un obiect de tip `AST` și se apelează funcția `parse` pentru a transforma vectorul de atomi lexicali într-un Arbore de Sintaxă Abstractă (AST).

5. **Afișarea Atomilor Lexicali și a Arborelui de Sintaxă Abstractă (opțional):**
   - Dacă flag-urile `--tokens` sau `--ast` sunt activate, se afișează corespunzător vectorul de atomi lexicali sau arborele de sintaxă abstractă.

6. **Traducerea și Afișarea Codului C++ Generat:**
   - Se apelează funcția `ast_to_c` din clasa `Translator` pentru a genera codul C++ corespunzător arborelui de sintaxă abstractă.
   - Codul rezultat este afișat în terminal.

Prin urmare, funcția `main` acționează ca un orchestrator care gestionează fluxul de execuție al programului, coordonând interacțiunea între diferitele componente ale acestuia (Lexer, AST, Translator) pentru a realiza analiza lexicală, sintactică și generarea de cod C++.

