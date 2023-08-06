struct inputs {
    int choice;
    double r;
    int n;
    int X<>;
    int Y<>;
};

struct averages {
    double avg<2>;
};

struct mul_arr {
    double array<>;
};


program ASK1_PROG {
    version ASK1_VERS {
        int CALC_INNER_PR(inputs) = 1;
        averages  FIND_AVGS(inputs) = 2;
        mul_arr CALC_MUL(inputs) = 3;
    } = 1;
} = 0x23451111;