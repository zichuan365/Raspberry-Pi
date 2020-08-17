#include "tranferProcess.h"



tranferProcess::tranferProcess()
{
    //set camera params
    cam.set( CV_CAP_PROP_FORMAT, CV_8UC1);
    cam.set(CV_CAP_PROP_EXPOSURE,-1);
    //cam.setExposureCompensation(-10);
    //set image size
    cam.set(cv::CAP_PROP_FRAME_WIDTH,2592);
    cam.set(cv::CAP_PROP_FRAME_HEIGHT,1944);
    //cam.set(cv::CAP_PROP_FRAME_WIDTH,2048);
    //cam.set(cv::CAP_PROP_FRAME_HEIGHT,1536);
    if (!cam.open())
    {
        cout << "open cam failed!"<<endl;
        //char *er = "004";
        //serverSocket::sendBuff(er,4,1);
        //send(1, er, 4, 0);
        //exit;
    }
    else
    {
        cout << "open cam success!"<<endl;
    }

    templateImg = imread("/home/pi/template.bmp",0);

    if(templateImg.empty())
        cout<<"read template error!"<<endl;

    FileStorage fs("/home/pi/RoiArea.xml", FileStorage::READ);
    fs["X"] >> roiX;
    fs["Y"] >> roiY;
    fs["Width"] >> roiW;
    fs["Height"] >> roiH;
    fs["begin"] >> begin;
    fs["end"] >> end;
    fs["tubeWidth"] >> tubeWidth;
    fs.release();
}

tranferProcess::~tranferProcess()
{
    cam.release();
}

int tranferProcess::createServer(int Port)
{
    int socket;
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc = -1;
    uint8_t *query;
    int header_length;

    //创建Modbus Tcp环境
    ctx = modbus_new_tcp("192.168.178.100", Port);
    //创建报文
    query = (uint8_t*)malloc(MODBUS_TCP_MAX_ADU_LENGTH);

    //分配线圈，寄存器的数组
    mb_mapping = modbus_mapping_new(
        UT_BITS_ADDRESS + UT_BITS_NB,
        UT_INPUT_BITS_ADDRESS + UT_INPUT_BITS_NB,
        UT_REGISTERS_ADDRESS + UT_REGISTERS_NB,
        UT_INPUT_REGISTERS_ADDRESS + UT_INPUT_REGISTERS_NB);
    if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    //线圈
    for(int i = 0;i<UT_BITS_NB;i++){
        mb_mapping->tab_bits[i] = UT_BITS_TAB[i];
    }
    //input线圈
    for(int i = 0;i<UT_INPUT_BITS_NB;i++){
        mb_mapping->tab_input_bits[i] = UT_INPUT_BITS_TAB[i];
    }
    //寄存器
    for(int i = 0;i<UT_REGISTERS_NB;i++){
        mb_mapping->tab_registers[i] = UT_REGISTERS_TAB[i];
    }
    //寄存器
    for(int i = 0;i<UT_INPUT_REGISTERS_NB;i++){
        mb_mapping->tab_input_registers[i] = UT_INPUT_REGISTERS_TAB[i];
    }

    //从后端，检索当前表头长度
    header_length = modbus_get_header_length(ctx);

    //设置从站，从站序列号
    modbus_set_slave(ctx, SERVER_ID);
    //启动调试模式
    modbus_set_debug(ctx, TRUE);

    //设置超时相应时间
//    struct timeval response_timeout;
//    response_timeout.tv_sec = 1;
//    response_timeout.tv_usec = 0;
//    modbus_set_response_timeout(ctx,&response_timeout);

    //开始监听
    socket = modbus_tcp_listen(ctx, 5);
    modbus_tcp_accept(ctx, &socket);
    cout<<"accept !!"<<endl;

    for (;;) {
        //收到指示请求，并返回请求长度
        rc = modbus_receive(ctx, query);
        cout<<"以上为接收报文,报文长："<<rc<<endl;

        if (rc == -1) {
            /* Connection closed by the client or error */
            cout<<"Client断开连接！"<<endl;
            modbus_tcp_accept(ctx, &socket);
            continue;
        }
        //当外部在首个线圈写入0时，程序退出
        if(mb_mapping->tab_bits[0] ==0){
            cout<<"程序退出！"<<endl;
            break;
        }

        if (query[header_length] == 0x17) {
            cout<<"收到功能码："<<"0x17"<<endl;
            //规定发送功能码0x17报文：格式
            if (MODBUS_GET_INT16_FROM_INT8(query, header_length + 3)
                == UT_REGISTERS_NB_SPECIAL) {
                printf("Set an incorrect number of values\n");
                MODBUS_SET_INT16_TO_INT8(query, header_length + 3,
                                         UT_REGISTERS_NB_SPECIAL - 1);
            } else if (MODBUS_GET_INT16_FROM_INT8(query, header_length + 1)
                == UT_REGISTERS_ADDRESS_SPECIAL) {
                printf("Reply to this special register address by an exception\n");
                modbus_reply_exception(ctx, query,
                                       MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY);
                continue;
            }

            int slope = 0;
            double high = 0;
            int high1 = 0;
            int high2 = 0;
            //liquid high
            cam.grab();
            cam.retrieve(frame);
            temp =frame(Rect(roiX,roiY,roiW,roiH));
            input = otsu(temp);

            edgeDetect::AboveProcess(input,slope,begin,end);
            edgeDetect::belowProcess(input,templateImg,tubeWidth,high);

            high1 = (static_cast<int>(high*10)) /10;
            high2 = (static_cast<int>(high*10)) %10;

            //低位表示
            query[18]= slope;
            //前为液面高度的小数点前数值，后为小数点后数值
            query[20]= high1;
            query[22]= high2;
        }

        cout<<"回复报文："<<endl;
        rc = modbus_reply(ctx, query, rc, mb_mapping);
        cout<<"回复报文长度："<<rc<<endl;
        if (rc == -1) {
            cout<<"回复报文出错！！!"<<endl;
            break;
        }
    }
    //释放Modbus Tcp环境
    printf("Quit the loop: %s\n", modbus_strerror(errno));
    close(socket);
    modbus_mapping_free(mb_mapping);
    free(query);
    modbus_free(ctx);

    return 1;
}
