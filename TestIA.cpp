// File: main.cpp
// Build: g++ -std=gnu++23 -O2 -pipe -static -s main.cpp -o databalatro
#include <iostream>
#include <string>
#include <random>

using std::cout;
using std::cin;
using std::string;

// ----- Modelos de datos -----
struct Carta {
    char Palo;      // 'C','E','D','T'
    int  Categoria; // 1..13 (A=1, J=11, Q=12, K=13)
    int  Valor;     // 2..10, J/Q/K=10, A=11
    bool Jugada;    // true si ya se jugó en la ronda
};

struct BSTNode {
    Carta carta;
    BSTNode* left;
    BSTNode* right;
};

struct ListNode {
    Carta* carta; // apunta a Carta dentro del BST
    ListNode* next;
};

// ----- Utilidades pequeñas (sin STL adicional) -----
static inline int max_int(int a, int b){ return a > b ? a : b; }
static inline int min_int(int a, int b){ return a < b ? a : b; }

static bool parse_uint(const string& s, int& out) {
    if (s.empty()) return false;
    long long v = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];
        if (ch < '0' || ch > '9') return false;
        v = v * 10 + (ch - '0');
        if (v > 1000000000) return false;
    }
    out = (int)v;
    return true;
}

static int tokenize(const string& line, string* toks, int cap) {
    int n = 0;
    size_t i = 0, N = line.size();
    while (i < N) {
        while (i < N && (line[i] == ' ' || line[i] == '\t' || line[i] == '\r')) ++i;
        if (i >= N) break;
        if (n >= cap) break;
        size_t j = i;
        while (j < N && line[j] != ' ' && line[j] != '\t' && line[j] != '\r') ++j;
        toks[n++] = line.substr(i, j - i);
        i = j;
    }
    return n;
}

// ----- BST: crear, insertar, recorrer -----
static BSTNode* newNode(char palo, int cat) {
    BSTNode* n = new BSTNode();
    n->left = n->right = nullptr;
    n->carta.Palo = palo;
    n->carta.Categoria = cat;
    n->carta.Jugada = false;
    // Valor según reglas (A=11, J/Q/K=10, 2..10 su número)
    if (cat == 1) n->carta.Valor = 11;
    else if (cat >= 11) n->carta.Valor = 10;
    else n->carta.Valor = cat;
    return n;
}

static void bstInsert(BSTNode*& root, BSTNode* node) {
    if (!root) { root = node; return; }
    if (node->carta.Categoria < root->carta.Categoria) bstInsert(root->left, node);
    else bstInsert(root->right, node);
}

static void buildBalancedRange(BSTNode*& root, char palo, int lo, int hi) {
    if (lo > hi) return;
    int mid = (lo + hi) / 2;
    bstInsert(root, newNode(palo, mid));
    buildBalancedRange(root, palo, lo, mid - 1);
    buildBalancedRange(root, palo, mid + 1, hi);
}

static void inorderCollect(BSTNode* root, Carta** arr, int& idx) {
    if (!root) return;
    inorderCollect(root->left, arr, idx);
    arr[idx++] = &root->carta;
    inorderCollect(root->right, arr, idx);
}

static void inorderPrintMark(BSTNode* root, ListNode* hand) {
    if (!root) return;
    inorderPrintMark(root->left, hand);
    // 'x' si está en mano o ya jugada en la ronda (por qué: requisito de interfaz)
    bool inHand = false;
    for (ListNode* p = hand; p; p = p->next) if (p->carta == &root->carta) { inHand = true; break; }
    cout << root->carta.Categoria << root->carta.Palo << ( (inHand || root->carta.Jugada) ? "x " : " ");
    inorderPrintMark(root->right, hand);
}

static void freeBST(BSTNode* root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    delete root;
}

// ----- Lista enlazada: mazo y mano -----
static void listPushBack(ListNode*& head, ListNode*& tail, Carta* c) {
    ListNode* n = new ListNode();
    n->carta = c; n->next = nullptr;
    if (!head) head = tail = n;
    else { tail->next = n; tail = n; }
}

static Carta* deckPopFront(ListNode*& deck) {
    if (!deck) return nullptr;
    ListNode* n = deck;
    Carta* c = n->carta;
    deck = n->next;
    delete n;
    return c;
}

static int listSize(ListNode* head) {
    int s = 0; for (; head; head = head->next) ++s; return s;
}

static bool removeByPointer(ListNode*& head, Carta* target) {
    ListNode* prev = nullptr;
    ListNode* cur = head;
    while (cur) {
        if (cur->carta == target) {
            if (prev) prev->next = cur->next; else head = cur->next;
            delete cur;
            return true;
        }
        prev = cur; cur = cur->next;
    }
    return false;
}

// copia mano a arreglo y lo ordena por categoría DESC, estable para empates
static int copyHandSortedDesc(ListNode* hand, Carta** arr, int cap) {
    int n = 0;
    for (ListNode* p = hand; p && n < cap; p = p->next) arr[n++] = p->carta;
    // simple insertion sort por categoría desc (y por palo para orden estable)
    for (int i = 1; i < n; ++i) {
        Carta* key = arr[i];
        int j = i - 1;
        while (j >= 0 && (arr[j]->Categoria < key->Categoria ||
                          (arr[j]->Categoria == key->Categoria && arr[j]->Palo < key->Palo))) {
            arr[j+1] = arr[j]; --j;
        }
        arr[j+1] = key;
    }
    return n;
}

static bool inHand(ListNode* hand, Carta* c) {
    for (ListNode* p = hand; p; p = p->next) if (p->carta == c) return true;
    return false;
}

static void addToHandFromDeck(ListNode*& hand, ListNode*& deck, int count) {
    for (int i = 0; i < count; ++i) {
        Carta* c = deckPopFront(deck);
        if (!c) return;
        // push front para O(1)
        ListNode* n = new ListNode(); n->carta = c; n->next = hand; hand = n;
    }
}

static void freeList(ListNode*& head) {
    while (head) { ListNode* n = head; head = head->next; delete n; }
}

// ----- Barajar 52 cartas (sin vector) -----
static void shuffleDeck(Carta** deckArr, int N, std::mt19937& gen) {
    for (int i = N - 1; i > 0; --i) {
        std::uniform_int_distribution<int> dist(0, i);
        int j = dist(gen);
        Carta* tmp = deckArr[i];
        deckArr[i] = deckArr[j];
        deckArr[j] = tmp;
    }
}

// ----- Evaluación de manos -----
enum HandRank {
    HR_HIGH = 1, HR_PAIR = 2, HR_TWO_PAIR = 3, HR_TRIPS = 4, HR_STRAIGHT = 5, HR_FLUSH = 6, HR_FOUR = 7
};

struct EvalResult {
    HandRank rank;
    int puntos;       // base + suma de categorías relevantes
    const char* nombre;
};

static bool isFlush(Carta** five) {
    char p = five[0]->Palo;
    for (int i = 1; i < 5; ++i) if (five[i]->Palo != p) return false;
    return true;
}

static void sortCatsAsc(const int* in, int n, int* out) {
    for (int i = 0; i < n; ++i) out[i] = in[i];
    for (int i = 1; i < n; ++i) {
        int key = out[i], j = i - 1;
        while (j >= 0 && out[j] > key) { out[j+1] = out[j]; --j; }
        out[j+1] = key;
    }
}

static bool isStraight(Carta** five) {
    int cats[5];
    for (int i = 0; i < 5; ++i) cats[i] = five[i]->Categoria;
    int a[5]; sortCatsAsc(cats, 5, a);
    // duplicados invalidan escalera
    for (int i = 1; i < 5; ++i) if (a[i] == a[i-1]) return false;
    // A-2-3-4-5
    bool wheel = (a[0]==1 && a[1]==2 && a[2]==3 && a[3]==4 && a[4]==5);
    if (wheel) return true;
    // otra escalera sin considerar A alto (no requerido)
    for (int i = 1; i < 5; ++i) if (a[i] != a[0] + i) return false;
    return true;
}

static EvalResult eval5(Carta** five) {
    int freq[14]; for (int i = 0; i < 14; ++i) freq[i] = 0;
    for (int i = 0; i < 5; ++i) freq[five[i]->Categoria]++;
    int pairs = 0, trips = 0, four = 0;
    int pairVals[2]; int pv = 0;
    int tripVal = 0, fourVal = 0, highVal = 0;
    for (int v = 1; v <= 13; ++v) {
        if (freq[v] == 4) { four = 1; fourVal = v; }
        else if (freq[v] == 3) { trips = 1; tripVal = v; }
        else if (freq[v] == 2) { if (pv < 2) pairVals[pv++] = v; pairs++; }
        if (freq[v] >= 1) highVal = max_int(highVal, v);
    }

    bool flush = isFlush(five);
    bool straight = isStraight(five);

    // puntajes base
    const int BASE_HIGH = 50, BASE_PAIR = 40, BASE_2PAIR = 80, BASE_TRIPS = 120,
              BASE_STRAIGHT = 150, BASE_FLUSH = 200, BASE_FOUR = 400;

    EvalResult r;
    if (four) {
        int sum = 4 * fourVal;
        r = {HR_FOUR, BASE_FOUR + sum, "Póker"};
        return r;
    }
    if (flush) {
        int sum = 0; for (int i = 0; i < 5; ++i) sum += five[i]->Categoria;
        r = {HR_FLUSH, BASE_FLUSH + sum, "Color"};
        return r;
    }
    if (straight) {
        int sum = 0; for (int i = 0; i < 5; ++i) sum += five[i]->Categoria;
        r = {HR_STRAIGHT, BASE_STRAIGHT + sum, "Escalera"};
        return r;
    }
    if (trips) {
        int sum = 3 * tripVal;
        r = {HR_TRIPS, BASE_TRIPS + sum, "Tercia"};
        return r;
    }
    if (pairs >= 2) {
        int sum = 2 * (pairVals[0] + pairVals[1]);
        r = {HR_TWO_PAIR, BASE_2PAIR + sum, "Doble par"};
        return r;
    }
    if (pairs == 1) {
        int sum = 2 * pairVals[0];
        r = {HR_PAIR, BASE_PAIR + sum, "Par"};
        return r;
    }
    // carta más alta
    r = {HR_HIGH, BASE_HIGH + highVal, "Carta más alta"};
    return r;
}

// ----- UI -----
static void printHandLine(ListNode* hand) {
    Carta* arr[16];
    int n = copyHandSortedDesc(hand, arr, 16);
    for (int i = 0; i < n; ++i) {
        cout << arr[i]->Categoria << arr[i]->Palo << (i+1<n ? ' ' : '\n');
    }
    if (n == 0) cout << "(mano vacía)\n";
}

static void showAllCards(BSTNode** trees, ListNode* hand) {
    // Orden fijo de palos para mostrar (a elección): E, C, T, D
    const int order[4] = {1,0,3,2}; // 0:C,1:E,2:D,3:T
    for (int oi = 0; oi < 4; ++oi) {
        int s = order[oi];
        inorderPrintMark(trees[s], hand);
        cout << "\n";
    }
}

// ----- Juego -----
static void dealInitial(ListNode*& hand, ListNode*& deck, int target) {
    int need = target - listSize(hand);
    if (need > 0) addToHandFromDeck(hand, deck, need);
}

static void drawToTarget(ListNode*& hand, ListNode*& deck, int target) {
    int cur = listSize(hand);
    if (cur < target) addToHandFromDeck(hand, deck, target - cur);
}

static void buildAndShuffleDeck(BSTNode** trees, ListNode*& deckHead, std::mt19937& gen) {
    // recolectar 52 cartas inorden por cada palo
    Carta* arr[52]; int idx = 0;
    for (int i = 0; i < 4; ++i) inorderCollect(trees[i], arr, idx);
    // barajar
    shuffleDeck(arr, 52, gen);
    // construir lista
    deckHead = nullptr;
    ListNode* tail = nullptr;
    for (int i = 0; i < 52; ++i) listPushBack(deckHead, tail, arr[i]);
}

static void resetJugadas(BSTNode* root) {
    if (!root) return;
    resetJugadas(root->left);
    root->carta.Jugada = false;
    resetJugadas(root->right);
}

static int deckCount(ListNode* deck) { return listSize(deck); }

static void printRoundHeader(int roundIdx, int pozo) {
    if (roundIdx == 0) cout << "Comienza la ciega pequen\u00a4a, pozo a vencer: " << pozo << " fichas\n";
    else if (roundIdx == 1) cout << "Comienza la ciega grande, pozo a vencer: " << pozo << " fichas\n";
    else cout << "Comienza la ciega Jefe, pozo a vencer: " << pozo << " fichas\n";
}

static void printStatus(int fichas, int pozo, int deckRem, int J, int D, ListNode* hand) {
    cout << "\n" << fichas << " / " << pozo << " fichas\n";
    cout << deckRem << " / 52 cartas\n";
    cout << J << "J / " << D << "D\n";
    printHandLine(hand);
    cout << "Juega: ";
}

static int targetHandSizeForRound(int roundIdx) {
    // Modificador elegido: El grillete (mano -1 en la Jefe)
    return (roundIdx == 2) ? 7 : 8;
}

static void printWinRound(int roundIdx, int total, int pozo) {
    if (roundIdx == 0) cout << "Ciega pequen\u00a4a vencida : " << total << " / " << pozo << " fichas\n";
    else if (roundIdx == 1) cout << "Ciega grande vencida : " << total << " / " << pozo << " fichas\n";
    else cout << "Ciega Jefe vencida : " << total << " / " << pozo << " fichas\n";
}

static void printLoseRound(int roundIdx, int total, int pozo) {
    if (roundIdx == 0) cout << "Ciega pequen\u00a4a perdida : " << total << " / " << pozo << " fichas\n";
    else if (roundIdx == 1) cout << "Ciega grande perdida : " << total << " / " << pozo << " fichas\n";
    else cout << "Ciega Jefe perdida : " << total << " / " << pozo << " fichas\n";
}

// ----- main -----
int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    cout << "Bienvenido al maravilloso juego de DataBalatro!\n";

    // 0:C,1:E,2:D,3:T (orden del arreglo a criterio)
    const char palos[4] = {'C','E','D','T'};
    BSTNode* trees[4] = {nullptr,nullptr,nullptr,nullptr};
    for (int i = 0; i < 4; ++i) buildBalancedRange(trees[i], palos[i], 1, 13);

    const int POZOS[3] = {250, 350, 500};
    for (int roundIdx = 0; roundIdx < 3; ++roundIdx) {
        // reset de banderas Jugada
        for (int i = 0; i < 4; ++i) resetJugadas(trees[i]);

        // construir y barajar mazo enlazado
        ListNode* deck = nullptr;
        buildAndShuffleDeck(trees, deck, gen);

        // inicializar mano y contadores
        ListNode* hand = nullptr;
        int targetSize = targetHandSizeForRound(roundIdx);
        dealInitial(hand, deck, targetSize);

        int J = 4; // manos por ronda
        int D = 3; // descartes por ronda
        int pozo = POZOS[roundIdx];
        int total = 0;

        // encabezado de ronda
        printRoundHeader(roundIdx, pozo);

        // bucle de acciones de la ronda
        string line;
        while (true) {
            printStatus(total, pozo, deckCount(deck), J, D, hand);

            // leer línea completa (soporta índices múltiples)
            if (!std::getline(cin, line)) break;
            if (line.size() == 0) { continue; }

            string toks[32];
            int nt = tokenize(line, toks, 32);
            if (nt <= 0) { continue; }

            if (toks[0] == "M" || toks[0] == "m") {
                // mostrar todas las cartas por palo (inorden), 'x' en mano/jugadas
                showAllCards(trees, hand);
                continue;
            }

            if (toks[0] == "D" || toks[0] == "d") {
                if (D <= 0) { cout << "Sin descartes disponibles.\n"; continue; }
                if (nt < 2) { cout << "Formato: D k idx0 idx1 ...\n"; continue; }
                int k = 0;
                if (!parse_uint(toks[1], k) || k < 0 || k > 5) { cout << "k invalido (0..5).\n"; continue; }
                if (nt != 2 + k) { cout << "Cantidad de indices no coincide con k.\n"; continue; }
                // construir vista ordenada para indexar
                Carta* view[16]; int nview = copyHandSortedDesc(hand, view, 16);
                if (k > nview) { cout << "No hay tantas cartas en mano.\n"; continue; }
                // leer índices y validar únicos y rango
                int idxs[8]; int used[16]; for (int i = 0; i < 16; ++i) used[i] = 0;
                bool bad = false;
                for (int i = 0; i < k; ++i) {
                    int ii;
                    if (!parse_uint(toks[2+i], ii) || ii < 0 || ii >= nview || used[ii]) { bad = true; break; }
                    used[ii] = 1; idxs[i] = ii;
                }
                if (bad) { cout << "Indices invalidos.\n"; continue; }
                // eliminar seleccionados por puntero
                for (int i = 0; i < k; ++i) {
                    Carta* sel = view[idxs[i]];
                    removeByPointer(hand, sel);
                    // NO marcar Jugada en descartes
                }
                D--;
                drawToTarget(hand, deck, targetSize);
                continue;
            }

            if (toks[0] == "J" || toks[0] == "j") {
                if (J <= 0) { cout << "Sin manos disponibles.\n"; continue; }
                if (nt < 2) { cout << "Formato: J 5 idx0 idx1 idx2 idx3 idx4\n"; continue; }
                int k = 0;
                if (!parse_uint(toks[1], k) || k != 5) { cout << "Debes jugar exactamente 5 cartas.\n"; continue; }
                if (nt != 7) { cout << "Se requieren 5 indices.\n"; continue; }

                Carta* view[16]; int nview = copyHandSortedDesc(hand, view, 16);
                if (nview < 5) { cout << "No tienes 5 cartas para jugar.\n"; continue; }

                int idxs[5]; int used[16]; for (int i = 0; i < 16; ++i) used[i] = 0;
                bool bad = false;
                for (int i = 0; i < 5; ++i) {
                    int ii;
                    if (!parse_uint(toks[2+i], ii) || ii < 0 || ii >= nview || used[ii]) { bad = true; break; }
                    used[ii] = 1; idxs[i] = ii;
                }
                if (bad) { cout << "Indices invalidos.\n"; continue; }

                Carta* sel[5];
                for (int i = 0; i < 5; ++i) sel[i] = view[idxs[i]];

                EvalResult er = eval5(sel);
                cout << "Mano jugada : " << er.nombre << "\n";
                cout << "Fichas anotadas : " << er.puntos << "\n";
                total += er.puntos;

                // marcar jugadas y remover de mano
                for (int i = 0; i < 5; ++i) sel[i]->Jugada = true;
                for (int i = 0; i < 5; ++i) removeByPointer(hand, sel[i]);

                J--;
                if (total >= pozo) {
                    printWinRound(roundIdx, total, pozo);
                    freeList(hand);
                    freeList(deck);
                    break; // siguiente ronda
                }
                drawToTarget(hand, deck, targetSize);
                continue;
            }

            cout << "Comando invalido. Usa M | D k idxs | J 5 idxs\n";
        }

        // si no se rompió por ganar, evaluar derrota
        if (/*aún en la ronda*/ true) {
            if (total < pozo) {
                printLoseRound(roundIdx, total, pozo);
                // liberar listas si no se liberaron
                freeList(hand);
                freeList(deck);
                // fin del juego
                break;
            }
        }

        if (roundIdx == 2 && total >= pozo) {
            cout << "Juego completado. ¡Ganaste todas las ciegas!\n";
        }
    }

    // liberar árboles
    for (int i = 0; i < 4; ++i) freeBST(trees[i]);
    return 0;
}
