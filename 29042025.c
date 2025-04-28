#include <stdio.h>      
#include <stdlib.h>    
#include <string.h>     

#define MAX_USERS 1000  
#define MAX_FRIENDS 100 

// ============================ Yapi Verileri ============================

typedef struct User {
    int id;                             // Kimlik kullanicisi
    struct User* friends[MAX_FRIENDS];  // Arkadaslara isaret�i dizisi
    int friendCount;                    // Kullanici arkadas sayisi
} User;

typedef struct Graph {
    User* users[MAX_USERS]; // T�m kullanicilara isaret�i dizisi
    int userCount;          // Grafikteki kullanici sayisi
} Graph;

typedef enum { RED, BLACK } Color; // Kirmizi-Siyah Aga� rengi i�in enum

typedef struct RBTreeNode {
    int id;                        // Kimlik d�g�m�
    Color color;                   // renk d�g�m�
    struct RBTreeNode *left, *right, *parent; // Sol, sag ve ebeveyn �ocuklara isaret�i
} RBTreeNode;

// ============================ Temel Islev ============================

// Belirli bir ID ile yeni bir kullanici olusturma
User* createUser(int id) {
    User* user = (User*)malloc(sizeof(User)); // Bellek tahsisi
    user->id = id;                            // Kimligi Ayarla
    user->friendCount = 0;                    // Baslangi�ta hi� arkadasi yoktu.
    return user;
}

// Grafige kullanici ekleme
void addUser(Graph* graph, int id) {
    User* user = createUser(id);             // Yeni bir kullanici olusturun
    graph->users[graph->userCount++] = user;  // Kullanicilar dizisine ekle
}

// Kimlige g�re kullanici arama
User* findUser(Graph* graph, int id) {
    int i;
    for (i = 0; i < graph->userCount; i++) {
        if (graph->users[i]->id == id)
            return graph->users[i];           // Bulunursa geri d�n
    }
    return NULL;                             // Bulunamadi
}

// Iki kullanici arasinda arkadaslik iliskisi ekleme
void addFriend(Graph* graph, int id1, int id2) {
    User* user1 = findUser(graph, id1);       // Ilk kullaniciyi bulun
    User* user2 = findUser(graph, id2);       // Ikinci kullaniciyi arayin

    if (user1 && user2) {                     // Her ikisi de mevcutsa
        user1->friends[user1->friendCount++] = user2; // Kullanici2'yi kullanici1'in arkadaslarina ekle
        user2->friends[user2->friendCount++] = user1; // kullanici1'i kullanici2'nin arkadaslarina ekle
    }
}

// ============================ DFS (Arkadas Arama) ============================

// Depth-First Search mesafe siniri i�inde arkadas bulmak i�in
void dfs(User* user, int* visited, int depth, int maxDepth) {
    int i;
    if (!user || visited[user->id] || depth > maxDepth)
        return; // Kullanici NULL ise, ziyaret edilmisse veya mesafeyi asmissa �ikar

    visited[user->id] = 1; // Kullaniciyi ziyaret edildi olarak isaretle
    printf("Kullanici %d, mesafe %d\n", user->id, depth); // Kullaniciyi ve mesafeyi g�ster

    for (i = 0; i < user->friendCount; i++) {
        dfs(user->friends[i], visited, depth + 1, maxDepth); // Arkadaslara geri d�n�s
    }
}

// ============================ Ortak Arkadas Analizi ============================

// Iki kullanici arasindaki ortak arkadas sayisini sayma
int countMutualFriends(User* user1, User* user2) {
    int count = 0;
    int i, j;
    for (i = 0; i < user1->friendCount; i++) {
        for (j = 0; j < user2->friendCount; j++) {
            if (user1->friends[i]->id == user2->friends[j]->id) {
                count++; // Ayni arkadas kimligine sahipseniz ekleyin
            }
        }
    }
    return count; // Ortak arkadas sayisini d�nd�r
}

// ============================ Topluluk Tespiti ============================

// Ayni toplulukta bulunan kullanicilari etiketlemek i�in DFS
void dfsMarkCommunity(User* user, int* visited) {
    int i;
    if (!user || visited[user->id]) return; // Zaten ziyaret edilmisse veya NULL ise, dur

    visited[user->id] = 1; // Yer imlerine eklendi, ziyaret edildi
    printf("%d ", user->id); // Kullanici kimligini yazdir

    for (i = 0; i < user->friendCount; i++) {
        dfsMarkCommunity(user->friends[i], visited); // Arkadaslara geri d�n�s
    }
}

// T�m topluluklari bulun ve g�r�nt�leyin
void findCommunities(Graph* graph) {
    int visited[MAX_USERS] = {0}; // Ziyaret edilen kullanicilari isaretlemek i�in dizi
    int i;

    printf("\nBulunan Topluluklar:\n");

    for (i = 0; i < graph->userCount; i++) {
        if (!visited[graph->users[i]->id]) {  // Eger ziyaret etmediyseniz
            printf("Topluluk: ");
            dfsMarkCommunity(graph->users[i], visited); // DFS'yi Baslat
            printf("\n");
        }
    }
}

// ============================ Kirmizi-Siyah Aga� ============================

// Yeni bir Kirmizi-Siyah Aga� d�g�m� olusturun
RBTreeNode* createRBNode(int id) {
    RBTreeNode* node = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    node->id = id;          // Kimligi Ayarla
    node->color = RED;      // Aslen kirmizi
    node->left = node->right = node->parent = NULL; // �ocuk ve ebeveyn NULL
    return node;
}

// Red-Black Tree - Sol Rotasyon
void leftRotate(RBTreeNode** root, RBTreeNode* x) {
    RBTreeNode* y = x->right;          // y, x'in sag �ocugudur
    x->right = y->left;                // y'nin sol �ocugu x'in sag �ocugu olur
    if (y->left) y->left->parent = x;  // Ebeveyni g�ncelle
    y->parent = x->parent;             // y, x'in konumunu degistirir
    if (!x->parent)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;                       // x, y'nin sol �ocugu olur
    x->parent = y;
}


// Red-Black Tree - Sag Rotasyon
void rightRotate(RBTreeNode** root, RBTreeNode* y) {
    RBTreeNode* x = y->left;            // x, y'nin sol �ocugudur
    y->left = x->right;                 // x'in sag �ocugu y'nin sol �ocugu olur
    if (x->right) x->right->parent = y; // Ebeveyni g�ncelle
    x->parent = y->parent;              // x, y konumunun yerini alir
    if (!y->parent)
        *root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;
    x->right = y;                       // y, x'in sag �ocugu olur
    y->parent = x;
}

// Eklemeden sonra Kirmizi-Siyah Aga� d�zeltildi
void insertFixup(RBTreeNode** root, RBTreeNode* z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBTreeNode* y = z->parent->parent->right; // D�g�m Amca
            if (y && y->color == RED) { // Vaka 1: Kizil Amca
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else { // Vaka 2/3: siyah amca
                if (z == z->parent->right) { // Vaka 2
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;   // Vaka 3
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else { // Sag taraf i�in simetrik
            RBTreeNode* y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK; // K�k her zaman siyahtir
}

// Kirmizi-Siyah Agaca d�g�m ekleme
void insertRBTree(RBTreeNode** root, int id) {
    RBTreeNode* z = createRBNode(id); // Yeni bir d�g�m olusturun
    RBTreeNode* y = NULL;
    RBTreeNode* x = *root;

    while (x) { // Yeni d�g�m�n eklenecegi konumu bulun
        y = x;
        if (z->id < x->id)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (!y)
        *root = z; // Aga� bos
    else if (z->id < y->id)
        y->left = z;
    else
        y->right = z;

    insertFixup(root, z); // Eklemeden sonra d�zeltme
}

// Kirmizi-Siyah Aga�ta Kimlik Arama
RBTreeNode* searchRBTree(RBTreeNode* root, int id) {
    if (!root || root->id == id)
        return root; // Bulusma ya da bulusma yok
    if (id < root->id)
        return searchRBTree(root->left, id); // Sol tarafta arama yapin
    else
        return searchRBTree(root->right, id); // Sag tarafta arama yapin
}

// ============================ Veri Setlerini Okuma ============================

// veri k�mesi dosyasi okumak
void readDataset(Graph* graph, RBTreeNode** rbRoot, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Dosya a�ilamadi.\n"); // Dosya a�ilamadi
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) { // Her satiri okuyun
        if (strncmp(line, "KULLANICI", 8) == 0) {
            int id;
            sscanf(line, "KULLANICI %d", &id); //  ID kullanicisi okumak
            addUser(graph, id);
            insertRBTree(rbRoot, id); // Kirmizi-Siyah Agaca Ekle
        } else if (strncmp(line, "ARKADAS", 5) == 0) {
            int id1, id2;
            sscanf(line, "ARKADAS %d %d", &id1, &id2); // Arkadaslik iliskilerini okuyun
            addFriend(graph, id1, id2);
        }
    }

    fclose(file); // dosyayi kapat
}

// ============================ Ana Program ============================

int main() {
    Graph graph = {0}; // Grafik baslatma
    RBTreeNode* rbRoot = NULL; // Kirmizi-Siyah Aga� Baslatma

    readDataset(&graph, &rbRoot, "dataset.txt"); // Veri k�mesini okuyun

    printf("=== Belirli mesafede arkadas arama ===\n");
    int visited[MAX_USERS] = {0}; // Ziyaret edilen dizi
    int startId = 1;              // baslangi� kimligi
    int maxDepth = 2;             // Maksimum mesafe

    User* startUser = findUser(&graph, startId);
    if (startUser) {
        dfs(startUser, visited, 0, maxDepth); // DFS startUser'dan basliyor
    } else {
        printf("Kullanici %d bulunamadi.\n", startId);
    }

    printf("\n=== Ortak arkadas analizi ===\n");

    User* userA = findUser(&graph, 2);
    User* userB = findUser(&graph, 3);
    if (userA && userB) {
        int mutual = countMutualFriends(userA, userB);
        printf("%d ve %d arasinda %d ortak arkadas var.\n", userA->id, userB->id, mutual);
    }

    printf("\n=== Topluluklari belirleme ===\n");
    findCommunities(&graph);

    printf("\n=== Red-Black Tree ile arama ===\n");
    int searchId = 3;
    RBTreeNode* result = searchRBTree(rbRoot, searchId);
    if (result) {
        printf("%d numarali kullanici Red-Black Tree'de bulundu.\n", searchId);
    } else {
        printf("%d numarali kullanici bulunamadi.\n", searchId);
    }

    return 0;
}

