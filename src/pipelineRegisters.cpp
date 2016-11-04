#include "../include/mySimpleSim.h"

#define OPCODE_ADD 0
#define OPCODE_SUB 1
#define OPCODE_MUL 2
#define OPCODE_DIV 3
#define OPCODE_MOD 4
#define OPCODE_CMP 5
#define OPCODE_AND 6
#define OPCODE_OR 7
#define OPCODE_NOT 8
#define OPCODE_MOV 9
#define OPCODE_LSL 10
#define OPCODE_LSR 11
#define OPCODE_ASR 12
#define OPCODE_NOP 13
#define OPCODE_LD 14
#define OPCODE_ST 15
#define OPCODE_BEQ 16
#define OPCODE_BGT 17
#define OPCODE_B 18
#define OPCODE_CALL 19
#define OPCODE_RET 20

struct Control {
        bool isSt, isBeq, isBgt, isRet, isImmediate, isWb, isUBranch, isBranchTaken, isAdd, isSub;
        bool isCmp, isMul, isDiv, isMod, isAnd, isOr, isNot, isMov, isLd, isLsl, isLsr, isAsr, isCall;
        void update(word opcode, int I) {
            isAdd = opcode == OPCODE_ADD || opcode == OPCODE_LD || opcode == OPCODE_ST;
            isSub = opcode == OPCODE_SUB;
            isCmp = opcode == OPCODE_CMP;
            isMul = opcode == OPCODE_MUL;
            isDiv = opcode == OPCODE_DIV;
            isMod = opcode == OPCODE_MOD;
            isLsl = opcode == OPCODE_LSL;
            isLsr = opcode == OPCODE_LSR;
            isAsr = opcode == OPCODE_ASR;
            isOr = opcode == OPCODE_OR;
            isAnd = opcode == OPCODE_AND;
            isNot = opcode == OPCODE_NOT;
            isMov = opcode == OPCODE_MOV;
            isCall = opcode == OPCODE_CALL;
            isLd = opcode == OPCODE_LD;
            isUBranch = (opcode == OPCODE_B) || (opcode == OPCODE_CALL) || (opcode == OPCODE_RET);
            isBgt = opcode == OPCODE_BGT;
            isBeq = opcode == OPCODE_BEQ;
            isWb = opcode == OPCODE_ADD || isSub || isMul || isDiv || isMod || isAnd || isOr || isNot || isMov || isLd || isLsl || isLsr || isAsr || isCall;
            isImmediate = I;
        }
};

class IF_OF {
        struct data {
                int PC;
                word instruction;
        };
        data input;
        data output;
        bool bubble;

    public:
        void tick() {
            if (!bubble) output = input;
        }
        void update(int pc, word inst) {

            input.PC = pc;
            input.instruction = inst;
        }
        word getInstruction() const {
            return output.instruction;
        }

        int getPc() const {
            return output.PC;
        }
        bool hasBubble() const {
            return bubble;
        }

        void push_bubble() {
            bubble = true;
        }

};

class OF_EX {
        struct data {
                int PC;
                int branch_target;
                word A;
                word B;
                word operand2;
                word instruction;
                Control control;

        };
        data input;
        data output;
        bool bubble;
    public:
        void tick() {
            if (!bubble) output = input;
        }
        void update(int pc, word b_target, word a, word b, word op2, word inst, Control ctrl) {

            input.PC = pc;
            input.branch_target = b_target;
            input.A = a;
            input.B = b;
            input.operand2 = op2;
            input.instruction = inst;
            input.control = ctrl;
        }

        word getA() const {
            return output.A;
        }

        word getB() const {
            return output.B;
        }

        int getBranchTarget() const {
            return output.branch_target;
        }

        const Control& getControl() const {
            return output.control;
        }

        word getInstruction() const {
            return output.instruction;
        }

        word getOperand2() const {
            return output.operand2;
        }

        int getPc() const {
            return output.PC;
        }

        bool hasBubble() const {
            return bubble;
        }

        void push_bubble() {
            bubble = true;
        }
};

class EX_MA {
        struct data {
                int PC;
                int branchPC;
                word aluResult;
                word operand2;
                word instruction;
                Control control;
        };
        data input;
        data output;
        bool bubble;

    public:
        void tick() {
            if (!bubble) output = input;
        }
        void update(int pc, int b_PC, word alu, word op2, word inst, Control ctrl) {

            input.PC = pc;
            input.branchPC = b_PC;
            input.aluResult = alu;
            input.operand2 = op2;
            input.instruction = inst;
            input.control = ctrl;
        }

        word getAluResult() const {
            return output.aluResult;
        }

        word getbranchPC() const {
            return output.branchPC;
        }

        const Control& getControl() const {
            return output.control;
        }

        word getInstruction() const {
            return output.instruction;
        }

        word getOperand2() const {
            return output.operand2;
        }

        int getPc() const {
            return output.PC;
        }

        bool hasBubble() const {
            return bubble;
        }

        void push_bubble() {
            bubble = true;
        }
};

class MA_RW {
        struct data {
                int PC;
                int branchPC;
                word aluResult;
                word ldResult;
                word instruction;
                Control control;
        };
        data input;
        data output;
        bool bubble;

    public:
        void tick() {
            if (!bubble) output = input;
        }

        void update(int pc, int b_PC, word ld_rslt, word alu, word inst, Control ctrl) {
            input.PC = pc;
            input.branchPC = b_PC;
            input.aluResult = alu;
            input.ldResult = ld_rslt;
            input.instruction = inst;
            input.control = ctrl;
        }

        word getAluResult() const {
            return output.aluResult;
        }

        const Control& getControl() const {
            return output.control;
        }

        word getInstruction() const {
            return output.instruction;
        }

        word getLdResult() const {
            return output.ldResult;
        }

        int getPc() const {
            return output.PC;
        }

        word getbranchPC() const {
            return output.branchPC;
        }

        bool hasBubble() const {
            return bubble;
        }

        void push_bubble() {
            bubble = true;
        }

};

