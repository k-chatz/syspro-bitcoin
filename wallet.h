#ifndef WALLET_H
#define WALLET_H

#include "list.h"

struct Wallet {
    char *userId;
    listPtr bitcoins;
    //todo transactions list
};

#endif //WALLET_H
