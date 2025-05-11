void bar() {
    printf("In bar\n");
}

void foo() {
    bar();
    printf("Back in foo\n");
}

int main() {
    foo();
    return 0;
}
