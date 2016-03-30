#pragma once
#include "TcpClient.h"
#include "Servo.h"
#include <QList>
#include <QTimer>

/**
 * @brief fpga通信接口
 */

class RobotClient : public TcpClient
{
    Q_OBJECT
public:
    typedef struct{
        quint32 ioOut;
        quint32 ioIn;
        quint8  reserved1;
        quint8  reserved2;
        quint8  motorRunning:1;      ///< 电机是否在运动(硬件fifo是否为非0)
        quint8  timeSynchronized:1;  ///< 时间同步成功
        quint8  motorError0:1;       ///< 电机0警告输出
        quint8  motorError1:1;       ///< 电机1警告输出
        quint8  motorError2:1;       ///< 电机2警告输出
        quint8  motorError3:1;       ///< 电机3警告输出
        quint8  motorError4:1;       ///< 电机4警告输出
        quint8  motorError5:1;       ///< 电机5警告输出
        quint8  motorEnabled:1;      ///< 是否电机使能
        quint8  motorPowered:1;      ///< 电机是否上电
        quint8  motorUnbraked:1;     ///< 是否电机解除抱闸
        quint8  emergencyPressed:1;  ///< 紧急停止按钮按下
        quint8  reserved4:4;
    }ActorState;
    ActorState actorState;

    BGDServo servos[MAX_AXIS];
    FrameHoster rs485;
    /**
     * @brief 管理通过BaseClient的协议来进行Modbus的访问
     */
    BGDServos servoManager;
    qint32 remoteFifoPayload;
    qint32 remoteFifoMaximum;
public:
    RobotClient();

    ActorState getActorState();
    bool getMotorError(qint32 id,qint32 *errcode=0);
    qint32 getRemoteFifoPayload();
    qint32 getRemoteFifoMaximum();

public slots:
    void rs485Send(QByteArray data);
    void setMotorEnable(bool enable);
    void setMotorPower(bool enable);
    void setMotorBrake(bool enable);
    void requestStatus();
    void requestFifoStatus();
    void clearTimeSync();
    void clearFifo();
    void asyncSetIO(quint32 io, quint32 mask=0xFFFFFFFF);
    void setTsIO(qint64 ts,quint32 io,quint32 mask);
    void syncTs(qint64 ts);

signals:
    void statusUpdated();
private slots:
    void onCmdReceived(int cmd,QByteArray data);  ///处理由fpga返回的指令
};




