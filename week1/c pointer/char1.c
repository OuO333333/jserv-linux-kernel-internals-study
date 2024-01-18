char *func(){
    char p[] = "hello world";
    return p;
}
int main(){
    printf("%p\n", func());
}
