#include "RobotClient.h"
#include "global/crc.h"
#include "Configure.h"

RobotClient::RobotClient()
{
    memset(&actorState,0,sizeof(ActorState));
    remoteFifoMaximum=0;
    remoteFifoPayload=0;
    connect(this,&RobotClient::cmdReceived,this,&RobotClient::onCmdReceived);
    {
        connect(&rs485,SIGNAL(dataNeedSend(QByteArray)),this,SLOT(rs485Send(QByteArray)));
        servoManager.addHoster(&rs485);
        for(int i=0;i<globalSettings->axisNum;i++){
            servos[i].devAddr=i+1;
            servos[i].parent=&rs485;
            servoManager.addServos(servos+i);
        }
    }
}

RobotClient::ActorState RobotClient::getActorState(){
    return actorState;
}

qint32 RobotClient::getRemoteFifoMaximum(){
    return remoteFifoMaximum;
}

qint32 RobotClient::getRemoteFifoPayload(){
    return remoteFifoPayload;
}

void RobotClient::onCmdReceived(int cmd,QByteArray data){
    switch(cmd){
        case GlobalCmd::GET_FIFO:{
            qint32 *lps=(qint32*)data.data();
            remoteFifoPayload=lps[1];
            remoteFifoMaximum=lps[2];
            break;
        }
        case GlobalCmd::GET_STATE:{
            ActorState *lps=(ActorState*)data.data();
            actorState=lps[0];
            emit statusUpdated();
            break;
        }
        case GlobalCmd::COMMUNICATE_RS485:
            rs485.processData(data);
            break;
        default:
            qWarning()<<"ActorClient::onCmdReceived():unhandled cmd="<<cmd<<"data="<<data;
            break;
    }
}

void RobotClient::rs485Send(QByteArray data){
    sendCmd(GlobalCmd::COMMUNICATE_RS485,data);
}
void RobotClient::setMotorEnable(bool enable){
    QByteArray data;
    data.fill('0',2);
    uint8_t *lpu8=(uint8_t*)data.data();
    lpu8[0]=enable?1:0;
    sendCmd(GlobalCmd::SET_SERVO_ENABLE,data);
}

void RobotClient::setMotorPower(bool enable){
    QByteArray data;
    data.fill('0',2);
    uint8_t *lpu8=(uint8_t*)data.data();
    lpu8[0]=enable?1:0;
    sendCmd(GlobalCmd::SET_SERVO_POWER,data);
}

void RobotClient::setMotorBrake(bool enable){
    QByteArray data;
    data.fill('0',2);
    uint8_t *lpu8=(uint8_t*)data.data();
    lpu8[0]=enable?1:0;
    sendCmd(GlobalCmd::SET_SERVO_BRAKE,data);
}

void RobotClient::requestStatus(){
    sendCmd(GlobalCmd::GET_STATE,QByteArray());
}
void RobotClient::requestFifoStatus(){
    sendCmd(GlobalCmd::GET_FIFO,QByteArray(4,'\0'));
}

void RobotClient::clearTimeSync(){
    sendCmd(GlobalCmd::CLR_TIMESYNC,QByteArray());
}

void RobotClient::clearFifo(){
    sendCmd(GlobalCmd::CLR_FIFO,QByteArray());
}
void RobotClient::asyncSetIO(quint32 io,quint32 mask){
    QByteArray data;
    data.append((char*)&io,sizeof(io));
    data.append((char*)&mask,sizeof(mask));
    sendCmd(GlobalCmd::IMEDIATE_SET_IO,data);
}

void RobotClient::setTsIO(qint64 ts,quint32 io,quint32 mask){
    QByteArray data;
    data.append((char*)&ts,sizeof(ts));
    data.append((char*)&io,sizeof(io));
    data.append((char*)&mask,sizeof(mask));
    sendCmd(GlobalCmd::SET_TS_IO,data);
}

void RobotClient::syncTs(qint64 ts){
    QByteArray data;
    data.append((char*)&ts,sizeof(ts));
    sendCmd(GlobalCmd::SYNC_TS,data);
}

bool RobotClient::getMotorError(qint32 id,qint32 *errcode){
    quint8 error=0;
    switch(id){
        case 0:
        error=actorState.motorError0;
        break;
        case 1:
        error=actorState.motorError1;
        break;
        case 2:
        error=actorState.motorError2;
        break;
        case 3:
        error=actorState.motorError3;
        break;
        case 4:
        error=actorState.motorError4;
        break;
        case 5:
        error=actorState.motorError5;
        break;
        default:
        error=0;
        break;
    }
    if(errcode && id>=0 && id<servoManager.servos.length()){
        errcode[0]=servoManager.servos.at(0)->errCode;
    }
    return error?true:false;
}
