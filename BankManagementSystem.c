#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BANK_COUNT 10
#define MAX 50


/*Bank Management V2 :

Kullanýcý, hangi hesaba para yatýracaðýný seçebilecek.

Kullanýcý hangi hesaptan para çekeceðini seçebilecek.

Kullanýcý 2 tane banka hesabý var mesela, bir banka hesabýný kapatabilecek. Ýçinde para var ise,
diðer hesaba aktarabilecek. Banka besabý yoksa kullanýcýyý bilgilendirecek, hesabýnýzý kapatamazsýnýz diye.
Eðer banka hesabýný kapatýrken kullanýcýnýn borcu varsa,
diðer banka hesaplarýndan borçlarýný kapatabilecek.*/


struct Bank {
    char name[MAX];
    int password;
    int balance;
};

struct User {
    char username[MAX];
    int password;
    int bank_count;
    struct Bank banks[BANK_COUNT];
};

static void deposit(struct User* user, int amount, int* acc1, int* acc2) {
    for (int i = 0; i < user->bank_count; i++) {
        if (i == *acc1 || i == *acc2) {
            user->banks[i].balance += amount;
            printf("%d Para yatirildi. Yeni bakiye (%s): %d\n", amount, user->banks[i].name, user->banks[i].balance);
        }
    }
}

static void withdraw(struct User* user, int amount, int* acc1, int* acc2) {

    for (int i = 0; i < user->bank_count; i++) {

        if (i == *acc1 || i == *acc2) {

            if (user->banks[i].balance >= amount) {

                user->banks[i].balance -= amount;
                printf("%d Para cekildi. Yeni bakiye (%s): %d\n", amount, user->banks[i].name, user->banks[i].balance);

            }
            else if (user->banks[i].balance < amount) {

                user->banks[i].balance -= amount;
                printf("%d Para cekildi. Yeni bakiye (%s}: %d\n", amount, user->banks[i].name, user->banks[i].balance);
                printf("%d TL borca girildi\n", user->banks[i].balance);
            }
        }
    }
}

static void closeAccAndDebt(struct User* user, int accIndex) {

    
    int otheraccIndex = (accIndex == 0) ? 1 : 0;  

    if (user->banks[accIndex].balance > 0) {

        printf("Hesabinizda para var kapanamaz..\n");
        printf("Bu nedenle paraniz aktariliyor..\n");

        printf("%s hesabinizdan diger %s hesabiniza %d paraniz aktarilmistir..\n",user->banks[accIndex].name, user->banks[otheraccIndex].name, user->banks[accIndex].balance);

        user->banks[otheraccIndex].balance += user->banks[accIndex].balance;
        user->banks[accIndex].balance = 0;
    }
    else if (user->banks[accIndex].balance < 0) {

        printf("Banka hesabinizin borcu var..\n");
        printf("Borcu kapatmak icin diger banka hesabinizdan islem yapiliyor.\n");

        int debt = -(user->banks[accIndex].balance); 

        if (user->banks[otheraccIndex].balance >= debt) {

            user->banks[otheraccIndex].balance -= debt;
            user->banks[accIndex].balance = 0; 

            printf("%s hesabinizdan %s hesabinizin borcu olan %d miktar aktarildi.\n",user->banks[otheraccIndex].name, user->banks[accIndex].name, debt);
        }
        else {

            printf("%s hesabinizda yeterli para yok, borç kapanamadi.\n", user->banks[otheraccIndex].name);

            return;
        }
    }

    printf("%s hesabi kapatildi.\n", user->banks[accIndex].name);
    //user->banks[accIndex].balance = 0;
    //user->banks[accIndex].password = 0;
    //user->banks[accIndex].name[0] = '\0'; 
}


int main() {
    struct User user1;
    user1.bank_count = 2;

    strcpy(user1.username, "ahmet");
    user1.password = 1234;

    strcpy(user1.banks[0].name, "ziraat");
    user1.banks[0].password = 1234;
    user1.banks[0].balance = 200;

    strcpy(user1.banks[1].name, "garanti");
    user1.banks[1].password = 1234;
    user1.banks[1].balance = 0;

    char input_name[MAX];
    int input_password;
    int choice;
    int amount;
    int acc1 = -1;
    int acc2 = -1;

    printf("Enter a name: ");
    scanf("%s", input_name);
    printf("Enter a password: ");
    scanf("%d", &input_password);



    if (strcmp(user1.username, input_name) == 0 && user1.password == input_password) {

        printf("Bankaya girildi..\n");

        while (1) {
            printf("1. Para yatirin.\n");
            printf("2. Para cekin.\n");
            printf("3. Hesap kapatin.\n");
            printf("4. cikis yapin\n");
            printf("Seciminiz: ");
            scanf("%d", &choice);

            if (choice == 1 || choice == 2) {
                printf("Birinci hesabi secin (ziraat: 0, garanti: 1): ");
                scanf("%d", &acc1);
                printf("Ikinci hesabi secin (ziraat: 0, garanti: 1) ya da -1 girin (tek hesap): ");
                scanf("%d", &acc2);

                if (choice == 1) {
                    printf("Yatirilacak miktar: ");
                    scanf("%d", &amount);
                    deposit(&user1, amount, &acc1, &acc2);
                }
                else if (choice == 2) {
                    printf("Cekilecek miktar: ");
                    scanf("%d", &amount);
                    withdraw(&user1, amount, &acc1, &acc2);
                }
            }
            else if (choice == 3) {
                printf("Hangi hesabi kapatmak istersiniz.. (ziraat: 0, garanti:1)\n");
                printf("Seciminiz:");
                scanf("%d", &choice);
                if (choice == 0) {
                    if (user1.banks[0].balance >= 0 || user1.banks[0].balance < 0) {

                        closeAccAndDebt(&user1, 0);
                    }
                    else {
                        printf("Hesabinizda para var kapatamazsiniz");
                    }
                }
                else if (choice == 1) {
                    if (user1.banks[1].balance >= 0 || user1.banks[1].balance < 0) {

                        closeAccAndDebt(&user1, 1);
                    }
                    else {
                        printf("Hesabinizda para var kapatamazsiniz..\n");
                    }
                }
            }
            else if (choice == 4) {
                printf("Cikis yapiliyor.\n");
                return 0;
            }
            else {
                printf("Gecersiz secim.\n");
            }
        }
    }
    else {
        printf("Hatali bilgi girdiniz!!\n");
    }

    return 0;
}