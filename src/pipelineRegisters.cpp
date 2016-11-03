#include "../include/mySimpleSim.h"

struct Control {
        bool isSt, isBeq, isBgt, isRet, isImmediate, isWb, isUBranch, isBranchTaken, isAdd, isSub;
        bool isCmp, isMul, isDiv, isMod, isAnd, isOr, isNot, isMov, isLd, isLsl, isLsr, isAsr, isCall;
        void update(word opcode, int I) {
            isAdd = opcode == 0 || opcode == 14 || opcode == 15;
            isSub = opcode == 1;
            isCmp = opcode == 5;
            isMul = opcode == 2;
            isDiv = opcode == 3;
            isMod = opcode == 4;
            isLsl = opcode == 10;
            isLsr = opcode == 11;
            isAsr = opcode == 12;
            isOr = opcode == 7;
            isAnd = opcode == 6;
            isNot = opcode == 8;
            isMov = opcode == 9;
            isCall = opcode == 19;
            isLd = opcode == 14;
            isUBranch = (opcode == 18) || (opcode == 19) || (opcode == 20);
            isBgt = opcode == 17;
            isBeq = opcode == 16;
            isWb = opcode == 0 || isSub || isMul || isDiv || isMod || isAnd || isOr || isNot || isMov || isLd || isLsl || isLsr || isAsr || isCall;
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
            output = input;
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
            output = input;
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
            output = input;
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


};

class MA_RW {
        struct data {
                int PC;
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
            output = input;
        }

        void update() {
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
        
        bool hasBubble() const {
            return bubble;
        }

};

