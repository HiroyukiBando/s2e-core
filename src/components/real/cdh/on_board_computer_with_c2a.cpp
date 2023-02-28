/*
 * @file on_board_computer_with_c2a.cpp
 * @brief Class to emulate on board computer with C2A flight software
 */

#include "on_board_computer_with_c2a.hpp"

#ifdef USE_C2A
#include "src_core/System/TaskManager/task_dispatcher.h"
#include "src_core/System/TimeManager/time_manager.h"
#include "src_core/System/WatchdogTimer/watchdog_timer.h"
#include "src_core/c2a_core_main.h"
#endif

std::map<int, UartPort*> OBC_C2A::com_ports_c2a_;
std::map<int, I2cPort*> OBC_C2A::i2c_com_ports_c2a_;
std::map<int, GpioPort*> OBC_C2A::gpio_ports_c2a_;

OBC_C2A::OBC_C2A(ClockGenerator* clock_generator) : OnBoardComputer(clock_generator), timing_regulator_(1) {
  // Initialize();
}

OBC_C2A::OBC_C2A(ClockGenerator* clock_generator, int timing_regulator) : OnBoardComputer(clock_generator), timing_regulator_(timing_regulator) {
  // Initialize();
}

OBC_C2A::OBC_C2A(int prescaler, ClockGenerator* clock_generator, int timing_regulator, PowerPort* power_port)
    : OnBoardComputer(prescaler, clock_generator, power_port), timing_regulator_(timing_regulator) {
  // Initialize();
}

OBC_C2A::~OBC_C2A() {}

void OBC_C2A::Initialize() {
#ifdef USE_C2A
  TMGR_init();  // Time Manager
                // Initialize at the beginning in order to measure the execution
                // time of C2A core initialization.
  C2A_core_init();
  WDT_init();  // Watchdog timer. In SILS, it does not have meaning.

  TMGR_clear();  // This called in C2A_core_init, but should be called again
                 // just before executing the C2A main loop.
#endif
}

void OBC_C2A::MainRoutine(const int time_count) {
  UNUSED(time_count);

#ifdef USE_C2A
  if (is_initialized == false) {
    is_initialized = true;
    Initialize();
  }
  for (int i = 0; i < timing_regulator_; i++) {
    TMGR_count_up_master_clock();  // The update time oc C2A clock should be
                                   // 1msec
    TDSP_execute_pl_as_task_list();
  }
#endif
}

// Override functions
int OBC_C2A::ConnectComPort(int port_id, int tx_buffer_size, int rx_buffer_size) {
  if (com_ports_c2a_[port_id] != nullptr) {
    // Port already used
    return -1;
  }
  com_ports_c2a_[port_id] = new UartPort(tx_buffer_size, rx_buffer_size);
  return 0;
}

// Close port and free resources
int OBC_C2A::CloseComPort(int port_id) {
  // Port not used
  if (com_ports_c2a_[port_id] == nullptr) return -1;

  UartPort* port = com_ports_c2a_.at(port_id);
  delete port;
  com_ports_c2a_.erase(port_id);
  return 0;
}

int OBC_C2A::SendFromObc(int port_id, unsigned char* buffer, int offset, int length) {
  return OBC_C2A::SendFromObc_C2A(port_id, buffer, offset, length);
}

int OBC_C2A::ReceivedByCompo(int port_id, unsigned char* buffer, int offset, int length) {
  UartPort* port = com_ports_c2a_[port_id];
  if (port == nullptr) return -1;
  return port->ReadTx(buffer, offset, length);
}

int OBC_C2A::SendFromCompo(int port_id, unsigned char* buffer, int offset, int length) {
  UartPort* port = com_ports_c2a_[port_id];
  if (port == nullptr) return -1;
  return port->WriteRx(buffer, offset, length);
}

int OBC_C2A::ReceivedByObc(int port_id, unsigned char* buffer, int offset, int length) {
  return OBC_C2A::ReceivedByObc_C2A(port_id, buffer, offset, length);
}

// Static functions
int OBC_C2A::SendFromObc_C2A(int port_id, unsigned char* buffer, int offset, int length) {
  UartPort* port = com_ports_c2a_[port_id];
  if (port == nullptr) return -1;
  return port->WriteTx(buffer, offset, length);
}
int OBC_C2A::ReceivedByObc_C2A(int port_id, unsigned char* buffer, int offset, int length) {
  UartPort* port = com_ports_c2a_[port_id];
  if (port == nullptr) return -1;
  return port->ReadRx(buffer, offset, length);
}

// If the character encoding of C2A is UTF-8, these functions are not necessary,
// and users can directory use SendFromObc_C2A and ReceivedByObc_C2A
int OBC_C2A_SendFromObc(int port_id, unsigned char* buffer, int offset, int length) {
  return OBC_C2A::SendFromObc_C2A(port_id, buffer, offset, length);
}
int OBC_C2A_ReceivedByObc(int port_id, unsigned char* buffer, int offset, int length) {
  return OBC_C2A::ReceivedByObc_C2A(port_id, buffer, offset, length);
}

int OBC_C2A::I2cConnectPort(int port_id, const unsigned char i2c_address) {
  if (i2c_com_ports_c2a_[port_id] != nullptr) {
    // Port already used
  } else {
    i2c_com_ports_c2a_[port_id] = new I2cPort();
  }
  i2c_com_ports_c2a_[port_id]->RegisterDevice(i2c_address);

  return 0;
}

int OBC_C2A::I2cCloseComPort(int port_id) {
  // Port not used
  if (i2c_com_ports_c2a_[port_id] == nullptr) return -1;

  I2cPort* port = i2c_com_ports_c2a_.at(port_id);
  delete port;
  i2c_com_ports_c2a_.erase(port_id);
  return 0;
}

int OBC_C2A::I2cWriteCommand(int port_id, const unsigned char i2c_address, const unsigned char* data, const unsigned char length) {
  I2cPort* i2c_port = i2c_com_ports_c2a_[port_id];
  i2c_port->WriteCommand(i2c_address, data, length);
  return 0;
}

int OBC_C2A::I2cWriteRegister(int port_id, const unsigned char i2c_address, const unsigned char* data, const unsigned char length) {
  I2cPort* i2c_port = i2c_com_ports_c2a_[port_id];

  if (length == 1) {
    i2c_port->WriteRegister(i2c_address, data[0]);
  } else {
    for (unsigned char i = 0; i < length - 1; i++) {
      i2c_port->WriteRegister(i2c_address, data[0] + i, data[i + 1]);
    }
  }
  return 0;
}

int OBC_C2A::I2cReadRegister(int port_id, const unsigned char i2c_address, unsigned char* data, const unsigned char length) {
  I2cPort* i2c_port = i2c_com_ports_c2a_[port_id];
  for (int i = 0; i < length; i++) {
    data[i] = i2c_port->ReadRegister(i2c_address);
  }
  return 0;
}

int OBC_C2A::I2cComponentWriteRegister(int port_id, const unsigned char i2c_address, const unsigned char reg_address, const unsigned char* data,
                                       const unsigned char length) {
  I2cPort* i2c_port = i2c_com_ports_c2a_[port_id];
  for (unsigned char i = 0; i < length; i++) {
    i2c_port->WriteRegister(i2c_address, reg_address + i, data[i]);
  }
  return 0;
}
int OBC_C2A::I2cComponentReadRegister(int port_id, const unsigned char i2c_address, const unsigned char reg_address, unsigned char* data,
                                      const unsigned char length) {
  I2cPort* i2c_port = i2c_com_ports_c2a_[port_id];
  for (unsigned char i = 0; i < length; i++) {
    data[i] = i2c_port->ReadRegister(i2c_address, reg_address + i);
  }
  return 0;
}
int OBC_C2A::I2cComponentReadCommand(int port_id, const unsigned char i2c_address, unsigned char* data, const unsigned char length) {
  I2cPort* i2c_port = i2c_com_ports_c2a_[port_id];
  i2c_port->ReadCommand(i2c_address, data, length);
  return 0;
}

int OBC_C2A_I2cWriteCommand(int port_id, const unsigned char i2c_address, const unsigned char* data, const unsigned char length) {
  return OBC_C2A::I2cWriteCommand(port_id, i2c_address, data, length);
}
int OBC_C2A_I2cWriteRegister(int port_id, const unsigned char i2c_address, const unsigned char* data, const unsigned char length) {
  return OBC_C2A::I2cWriteRegister(port_id, i2c_address, data, length);
}
int OBC_C2A_I2cReadRegister(int port_id, const unsigned char i2c_address, unsigned char* data, const unsigned char length) {
  return OBC_C2A::I2cReadRegister(port_id, i2c_address, data, length);
}

int OBC_C2A::GpioConnectPort(int port_id) {
  if (gpio_ports_c2a_[port_id] != nullptr) {
    // Port already used
    return -1;
  }
  gpio_ports_c2a_[port_id] = new GpioPort(port_id);
  return 0;
}

int OBC_C2A::GpioComponentWrite(int port_id, const bool is_high) {
  GpioPort* port = gpio_ports_c2a_[port_id];
  if (port == nullptr) return -1;
  return port->DigitalWrite(is_high);
}

bool OBC_C2A::GpioComponentRead(int port_id) {
  GpioPort* port = gpio_ports_c2a_[port_id];
  if (port == nullptr) return false;
  return port->DigitalRead();
}

int OBC_C2A::GpioWrite_C2A(int port_id, const bool is_high) {
  GpioPort* port = gpio_ports_c2a_[port_id];
  if (port == nullptr) return -1;
  return port->DigitalWrite(is_high);
}

bool OBC_C2A::GpioRead_C2A(int port_id) {
  GpioPort* port = gpio_ports_c2a_[port_id];
  if (port == nullptr) return false;
  return port->DigitalRead();
}

int OBC_C2A_GpioWrite(int port_id, const bool is_high) { return OBC_C2A::GpioWrite_C2A(port_id, is_high); }

bool OBC_C2A_GpioRead(int port_id) { return OBC_C2A::GpioRead_C2A(port_id); }