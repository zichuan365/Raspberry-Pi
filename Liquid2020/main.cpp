#include <iostream>
#include <tranferProcess.h>

using namespace std;

int main()
{  

    //程序运行
    cout << "create server..." <<endl;
    tranferProcess soc;
    if (!soc.createServer(8888))
    {
        cout << "create server failed!"<<endl;
        return 0;
    }

    //cout<<"服务已创建!"<<endl;
    while(true)
    {
        sleep(1);
    }

    return 0;
}
