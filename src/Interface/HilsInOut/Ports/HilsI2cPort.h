#pragma once
#include "HilsUartPort.h"

#include <map>

// I2C-USB�ϊ���̎g�p��z��
// �͋[�R���|��com�|�[�g����ĕϊ���Ƒ���M���s��

const int kDefaultCmdSize = 0xff;
const int kDefaultTxSize = 0xff; // TODO: change this value

class HilsI2cPort : public HilsUartPort // I2C-USB�ϊ���<->�͋[�R���|(Target)com�|�[�g������ʐM��UART�Ɠ��l
{
public:
  HilsI2cPort(const unsigned int port_id);
  HilsI2cPort(const unsigned int port_id, const unsigned char max_register_number);
  ~HilsI2cPort();

  void RegisterDevice();

  int WriteRegister(const unsigned char reg_addr);
  int WriteRegister(const unsigned char reg_addr, const unsigned char value);

  // unsigned char ReadRegister(const unsigned char i2c_addr);
  unsigned char ReadRegister( const unsigned char reg_addr);

  // int WriteCommand(const unsigned char i2c_addr, const unsigned char* tx_data, const unsigned int length);
  int ReadCommand(unsigned char* rx_data, const unsigned int length);

  int Receive();
  int Send(const unsigned char len);
  int GetStoredFrameCounter();

private:
  unsigned char max_register_number_ = 0xff;
  unsigned char saved_reg_addr_ = 0x00;
  unsigned int stored_frame_counter_ = 0; // �ϊ���ɂ��炩���ߐ��t���[�����̃e�����𑗐M���Ă���

  // < register address, value>
  std::map< unsigned char, unsigned char > device_registers_;

  // <cmd_buffer_length, value>
  std::map< unsigned char, unsigned char > cmd_buffer_;
};
