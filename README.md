# Social-Network-Analysis-System

## Genel Bakış

Bu proje, basit bir sosyal ağ analizini gerçekleştiren C dilinde yazılmış bir programdır. Program, kullanıcılar arasındaki arkadaşlık ilişkilerini graf yapısında temsil eder ve çeşitli sosyal ağ analiz algoritmaları uygular.

## Özellikler

- **Kullanıcı ve Arkadaşlık Yönetimi**: Sisteme kullanıcı ekleme ve kullanıcılar arasında arkadaşlık ilişkisi kurma
- **Derinlik Öncelikli Arama (DFS)**: Belirli bir kullanıcıdan başlayarak belirli mesafedeki arkadaşları bulma
- **Ortak Arkadaş Analizi**: İki kullanıcı arasındaki ortak arkadaşları sayma
- **Topluluk Tespiti**: Birbirine bağlı kullanıcı gruplarını (toplulukları) tanımlama
- **Kırmızı-Siyah Ağaç**: Verimli kullanıcı arama için gelişmiş veri yapısı

## Veri Yapıları

### Kullanıcı Yapısı
```c
typedef struct User {
    int id;                             // Kullanıcı kimliği
    struct User* friends[MAX_FRIENDS];  // Arkadaşlara işaretçi dizisi
    int friendCount;                    // Kullanıcı arkadaş sayısı
} User;
```

### Graf Yapısı
```c
typedef struct Graph {
    User* users[MAX_USERS]; // Tüm kullanıcılara işaretçi dizisi
    int userCount;          // Graftaki kullanıcı sayısı
} Graph;
```

### Kırmızı-Siyah Ağaç Yapısı
```c
typedef struct RBTreeNode {
    int id;                        // Düğüm kimliği
    Color color;                   // Düğüm rengi
    struct RBTreeNode *left, *right, *parent; // Sol, sağ ve ebeveyn çocuklara işaretçi
} RBTreeNode;
```

## Temel İşlevler

### Kullanıcı Yönetimi
- `createUser()`: Yeni bir kullanıcı oluşturur
- `addUser()`: Grafa kullanıcı ekler
- `findUser()`: Kimliğe göre kullanıcı arar
- `addFriend()`: İki kullanıcı arasında arkadaşlık ilişkisi ekler

### Sosyal Ağ Analizi
- `dfs()`: Belirli mesafedeki arkadaşları bulmak için derinlik öncelikli arama
- `countMutualFriends()`: İki kullanıcı arasındaki ortak arkadaşları sayar
- `dfsMarkCommunity()`: Topluluk üyelerini işaretlemek için DFS
- `findCommunities()`: Graftaki tüm toplulukları bulur

### Kırmızı-Siyah Ağaç İşlevleri
- `createRBNode()`: Yeni bir Kırmızı-Siyah Ağaç düğümü oluşturur
- `leftRotate()`, `rightRotate()`: Ağaç rotasyonları
- `insertFixup()`: Düğüm ekleme sonrası ağaç dengeleme
- `insertRBTree()`: Ağaca düğüm ekleme
- `searchRBTree()`: Ağaçta düğüm arama

## Veri Seti Formatı

Program, aşağıdaki formatta bir "dataset.txt" dosyasından veri okur:
```
KULLANICI [id]
ARKADAS [id1] [id2]
```

Örnek:
```
KULLANICI 1
KULLANICI 2
KULLANICI 3
ARKADAS 1 2
ARKADAS 2 3
```

## Programı Kullanma

1. Programı derleyin:
   ```
   gcc sosyal_ag.c -o sosyal_ag
   ```

2. "dataset.txt" dosyasını oluşturun ve kullanıcı ve arkadaşlık verilerini ekleyin

3. Programı çalıştırın:
   ```
   ./sosyal_ag
   ```

## Çıktı Örnekleri

Programın çıktıları şunları içerir:
- Belirli bir mesafedeki arkadaşların listesi
- İki kullanıcı arasındaki ortak arkadaş sayısı
- Tanımlanan topluluklar
- Kırmızı-Siyah Ağaç arama sonuçları

## Kısıtlamalar

- Maksimum 1000 kullanıcı desteklenir (MAX_USERS)
- Her kullanıcı için maksimum 100 arkadaş desteklenir (MAX_FRIENDS)
