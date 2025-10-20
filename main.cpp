#include <unistd.h>
#include <iostream>
#include <string.h>
void singlepipe(){
    int fds[2] = {-1,-1};
    char str[512]="hello";
    char buffer[512];
    pipe(fds);

    pid_t pid = fork();
    // pipe(fds);
    //这里在fork只会再创建管道，会在父子进程中创建两条完全独立的管道，但是问题是下面的输出(fds[0],fds[1])居然是一样的{3,4}
    //那内核是怎么区分这个{3,4}到底该指向那个管道对象呢？
    //文件描述符号（如 3、4）只是每个进程自己文件描述符表里的编号，内核根据“进程上下文 + 这个编号”去找对应的内核对象，因此父子进程的 3、4 看似相同，实则属于不同表、互不混淆。
    if (pid == 0){
        int len = write(fds[1],str,strlen(str)+1);
        // // write(fds[1],str,sizeof(str));//这里如果使用sizeof，则会发送512个字节过去，至于后面那些字节是什么内容无法保证！
        // printf("len:%d\n",len);
        printf("%d %d\n",fds[0],fds[1]);
    }else if (pid > 0){
        int len = read(fds[0],buffer,sizeof(buffer));//第三个参数表示最大可以接收多少字节
        printf("len_read:%d\n",len);
        printf("%s\n",buffer);
        pid_t pid2 = fork();
        if (pid2 == 0){
            // printf("hahahha\n");
            int len = read(fds[0],buffer,sizeof(buffer));//水(数据)已经被它的父进程接了(read)，管道里面是空的，同时write的进程也关闭了，所以这里直接返回0，读了空数据
            printf("%s\n",buffer);
        }
        // printf("%d %d\n",fds[0],fds[1]);
    }else exit(-1);
}
// void doublepipe(){
//     // 创建双管道，实现TCP的应答机制
//     int stoc[2]={-1,-1},ctos[2]={-1,-1};
//     pipe(stoc);
//     pipe(ctos);
//     char str[512];
//     pid_t pid = fork();
//     if (pid > 0){
//         fgets(str,sizeof(str),stdin);
//         write(stoc[1],str,strlen(str)+1);
//         memset(str,0,sizeof(str));
//         read(ctos[0],str,sizeof(str));
//         printf("%s",str);
//     }else if (pid == 0){
//         read(stoc[0],str,sizeof(str));
//         write(ctos[1],str,strlen(str)+1);
//     }else exit(-1);
// }
int main(){
    singlepipe();
    // doublepipe();
    return 0;
}