#ifndef PRINTSTATEMENT_H
#define PRINTSTATEMENT_H

#include "../../include/lexer/lexer.h"

class PrintStatement {
public:
    static void handlePrintStatement(Lexer &lexer);
};

#endif // PRINTSTATEMENT_H
