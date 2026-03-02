import sys
import serial
import random
import threading
from PyQt5 import QtWidgets, QtCore, QtGui
from serial.tools import list_ports
import pyqtgraph as pg
from collections import deque
import keyboard
import statistics
import time

array_size = 3008

ser = serial.Serial()

ser_open = False
ser_cond = threading.Condition()

rx_queue = deque(maxlen=4000)

rx_buffer = bytearray()

frames_queue = deque(maxlen=200)

def serial_thread():
    pack_size = 4+array_size*8+60*4+5
    def process_buffer(buf):
        if len(buf)<2*pack_size:
            return
        while True:
            idx = buf.find(b'\xee\xee')
            if idx<0:
                buf.clear()
                break
            
            if len(buf) < idx + pack_size:
                if idx > 0:
                    del buf[:idx]
                break

            frame = buf[idx:idx+pack_size]
            # print(frame)

            if frame[-2:] == b'\xff\xff':
                frames_queue.append(frame)
                del buf[:idx+pack_size]
            else:
                del buf[idx:idx+1]

    while True:
        with ser_cond:
            while not ser_open:
                ser_cond.wait()
                rx_buffer.clear()
        if ser.is_open:
            try:
                data = ser.read(pack_size)
            except Exception as e:
                print("Serial Error:",e)
                continue
            rx_buffer.extend(data)
            # print(rx_buffer)
            process_buffer(rx_buffer)

    # 第一步测试
    while True:
        while True:
            b = ser.read(1)
            rx_pack_back.append(b[0])
            if rx_pack_back==deque([0xFF,0xFF]):#结束符
                break

        read_raw = ser.read(pack_size)
        print(read_raw)

        raw = read_raw[0:pack_size]

        if raw[0]!=0xEE and raw[1]!=0xEE:
            print("pack head error")
            continue
        
        for i in range(2, array_size*8+2, single_size):
            com_input = raw[i:i+single_size]

            ch1 = (com_input[0]<<8)+com_input[1]
            ch2 = (com_input[2]<<8)+com_input[3]
            ch3 = (com_input[4]<<8)+com_input[5]
            ch4 = (com_input[6]<<8)+com_input[7]

            rx_queue.append((ch1,ch2,ch3,ch4))

            if raw[array_size*8+2]!=0xAB:
                print("wave head error")
                break
            # 24004=0xef,24005=len,24051=0xfe
            # 24052=0xef,24053=len,24099=0xfe
            # 24100=0xef,24101=len,24147=0xfe
            # 24148=0xef,24149=len,24195=0xfe
            for i in range(0, 4):
                frame_start = array_size*8+3+i*46

                rx_waves[i].clear()
                data_len = raw[frame_start]
                data_start = frame_start+1
                for j in range(data_len):
                    cell_start = data_start+j*3
                    get_high = raw[cell_start]
                    get_low = raw[cell_start+1]
                    get_dec = raw[cell_start+2]
                    get_wave = get_high*256+get_low+get_dec*0.01
                    rx_waves[i].append(get_wave)

        # if raw[pack_size-2]!=0xFF or raw[pack_size-1]!=0xFF:
        #     print("pack end error")

threading.Thread(target=serial_thread, daemon=True).start()

class MQComboBox(QtWidgets.QComboBox):
    def __init__(self):
        super().__init__()
        self.refresh_ports()

    def showPopup(self):
        self.refresh_ports()
        super().showPopup()

    def refresh_ports(self):
        current_text = self.currentText()

        self.blockSignals(True)
        self.clear()

        for p in list_ports.comports():
            text = f"{p.device} {p.description}"
            self.addItem(text, p.device)

        if current_text:
            index = self.findText(current_text)
            if index >= 0:
                self.setCurrentIndex(index)
        
        self.blockSignals(False)


class GraphWindow(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.paused = False
        self.process_down = True
        self.peaks_lines = [[] for _ in range(4)]
        self.visible_lines = [False for _ in range(4)]

        layout = QtWidgets.QGridLayout()
        self.setLayout(layout)

        self.ctrl_panel = QtWidgets.QWidget()
        self.ctrl_layout = QtWidgets.QHBoxLayout()
        self.ctrl_layout.setContentsMargins(5,5,5,5)
        self.ctrl_layout.setSpacing(10)
        self.ctrl_panel.setLayout(self.ctrl_layout)
        self.ctrl_panel.setMaximumHeight(80)

        lab_com = QtWidgets.QLabel("COM端口:")
        self.combo_com = MQComboBox()
        self.combo_com.setMinimumWidth(100)
        if self.combo_com.count() > 0:
            self.combo_com.setCurrentIndex(0)

        lab_baud = QtWidgets.QLabel("波特率:")
        self.combo_baud = QtWidgets.QComboBox()
        self.combo_baud.setMinimumWidth(100)
        self.combo_baud.addItems([
            "9600",
            "115200",
            "2000000",
            "3000000",
            "4000000",
            "6000000"
        ])
        self.combo_baud.setCurrentText("2000000")

        self.com_btn = QtWidgets.QPushButton("打开")
        self.com_btn.setMinimumWidth(100)

        self.clear_btn = QtWidgets.QPushButton("清空数据")
        self.clear_btn.setMinimumWidth(100)

        lab_interval = QtWidgets.QLabel("单数据间隔(us):")
        self.interval_text = QtWidgets.QTextEdit("10")
        self.interval_text.setMinimumWidth(100)
        self.interval_text.setMaximumHeight(25)

        self.combo_com.currentTextChanged.connect(self.on_com_changed)
        self.combo_baud.currentTextChanged.connect(self.on_baud_changed)
        self.com_btn.clicked.connect(self.on_open_changed)
        self.clear_btn.clicked.connect(self.on_clear_chart)
        self.interval_text.textChanged.connect(self.on_interval_changed)

        self.ctrl_layout.addWidget(lab_com)
        self.ctrl_layout.addWidget(self.combo_com)
        self.ctrl_layout.addSpacing(20)
        self.ctrl_layout.addStretch()

        self.ctrl_layout.addWidget(lab_baud)
        self.ctrl_layout.addWidget(self.combo_baud)
        self.ctrl_layout.addSpacing(20)
        self.ctrl_layout.addWidget(self.com_btn)
        self.ctrl_layout.addSpacing(20)
        self.ctrl_layout.addStretch()

        self.ctrl_layout.addWidget(self.clear_btn)
        self.ctrl_layout.addSpacing(20)
        self.ctrl_layout.addStretch()

        self.ctrl_layout.addWidget(lab_interval)
        self.ctrl_layout.addWidget(self.interval_text)
        
        layout.addWidget(self.ctrl_panel, 0, 0)

        self.plot1 = pg.PlotWidget()

        layout.addWidget(self.plot1, 1, 0)

        self.adc1 = deque(maxlen=array_size)
        self.adc2 = deque(maxlen=array_size)
        self.adc3 = deque(maxlen=array_size)
        self.adc4 = deque(maxlen=array_size)

        self.data1 = deque(maxlen=array_size)
        self.data2 = deque(maxlen=array_size)
        self.data3 = deque(maxlen=array_size)
        self.data4 = deque(maxlen=array_size)

        self.waves = [[0 for _ in range(15)] for _ in range(4)]

        self.plot1.setXRange(0,array_size)
        self.plot1.setYRange(0,6)
        self.plot1.showGrid(x=True, y=True)

        self.plot1.addLegend()
        self.curve1 = self.plot1.plot(pen='yellow', name='CH0')
        self.curve2 = self.plot1.plot(pen='green', name='CH1')
        self.curve3 = self.plot1.plot(pen='blue', name='CH2')
        self.curve4 = self.plot1.plot(pen='purple', name='CH3')

        self.num_panel = QtWidgets.QWidget()
        self.num_layout = QtWidgets.QGridLayout()
        self.num_panel.setLayout(self.num_layout)

        header_style = "font-weight:bold; font-size:14pt; padding:3pt;"

        self.check_boxs = []

        for r in range(4):
            cb = QtWidgets.QCheckBox()
            cb.setChecked(False)
            # cb.setAlignment(QtCore.Qt.AlignCenter)
            cb.stateChanged.connect(lambda state, i=r: self.toggle_line(i, state))
            self.check_boxs.append(cb)
            self.num_layout.addWidget(cb, r+1, 0)

        # 左侧表头（行：1~4）
        for r in range(4):
            lab = QtWidgets.QLabel(str(r+1))
            lab.setAlignment(QtCore.Qt.AlignCenter)
            lab.setStyleSheet(header_style)
            self.num_layout.addWidget(lab, r+1, 1)

        # 顶部表头（列：1~15）
        for c in range(15):
            lab = QtWidgets.QLabel(str(c+1))
            lab.setAlignment(QtCore.Qt.AlignCenter)
            lab.setStyleSheet(header_style)
            self.num_layout.addWidget(lab, 0, c+2)

        self.num_labels = [[None for _ in range(15)] for _ in range(4)]

        for r in range(4):
            for c in range(15):
                lab = QtWidgets.QLabel("0")
                lab.setAlignment(QtCore.Qt.AlignCenter)
                lab.setStyleSheet("font-size:14pt; padding:2pt;")
                self.num_labels[r][c] = lab
                self.num_layout.addWidget(lab, r+1, c+2)

        layout.addWidget(self.num_panel, 2, 0, 1, 2)

        self.port = self.combo_com.currentData()
        self.baud = int(self.combo_baud.currentText())
        self.interval = int(self.interval_text.toPlainText())
        global ser
        ser.port = self.port
        ser.baudrate = self.baud
        
        self.frame_timer = QtCore.QTimer()    
        self.frame_timer.timeout.connect(self.process_frame)
        
        self.update_timer = QtCore.QTimer()    
        self.update_timer.timeout.connect(self.update_plot)
        
        QtWidgets.QShortcut(QtGui.QKeySequence("P"), self, activated=self.toggle_pause)

    def on_com_changed(self):
        self.port = self.combo_com.currentData()
        self.update_serial()

    def on_baud_changed(self):
        self.baud = int(self.combo_baud.currentText())
        self.update_serial()
    
    def update_serial(self):
        global ser
        ser.port = self.port
        ser.baudrate = self.baud

    def on_open_changed(self):
        global ser_open
        global ser_cond
        global ser

        do_open = False
        do_close = False

        with ser_cond: 
            if ser_open == False:
                do_open = True
                ser_open = True 
                ser_cond.notify_all() 
                
            else: 
                do_close = True
                ser_open = False

        if do_open:
            try:
                ser.open()
            except Exception as e:
                print("Serial Error:",e)
                with ser_cond:
                    ser_open = False
                return
            
            self.combo_com.setEnabled(False)
            self.combo_baud.setEnabled(False)
            self.clear_btn.setEnabled(False)
            self.on_interval_changed()

            self.frame_timer.start(1)
            self.update_timer.start(2)

            self.com_btn.setText("关闭")
        elif do_close:
            try:
                ser.close()
            except Exception as e:
                print("Serial Error:",e)
                with ser_cond:
                    ser_open = True
                return
            self.combo_com.setEnabled(True)
            self.combo_baud.setEnabled(True)
            self.clear_btn.setEnabled(True)

            self.frame_timer.stop()
            self.update_timer.stop()

            self.com_btn.setText("打开")

    def on_clear_chart(self):
        curves = [self.curve1, self.curve2, self.curve3, self.curve4]
        for idx in range(4):
            curves[idx].setData([])
            for j in range(len(self.peaks_lines[idx])):
                self.peaks_lines[idx][j].setVisible(False)
                del self.peaks_lines[idx][j]
            for label in self.num_labels[idx]:  
                label.setText("0")

    def on_interval_changed(self):
        self.interval = int(self.interval_text.toPlainText())
        command_frame = [0]*8
        command_frame[0] = 0xFF
        command_frame[1] = 0xFF
        command_frame[2] = 0x01
        command_frame[6] = (self.interval>>8) & 0xFF
        command_frame[7] = self.interval & 0xFF
        ser.write(bytes(command_frame))

    def process_frame(self):
        if len(frames_queue)==0 or self.process_down==False:
            return
        raw = frames_queue.popleft()
        self.process_down = False
        single_size = 8

        if raw[0]!=0xEE and raw[1]!=0xEE:
            print("pack head error")
        
        for i in range(2, array_size*8+2, single_size):
            com_input = raw[i:i+single_size]

            ch1 = (com_input[0]<<8)+com_input[1]
            ch2 = (com_input[2]<<8)+com_input[3]
            ch3 = (com_input[4]<<8)+com_input[5]
            ch4 = (com_input[6]<<8)+com_input[7]

            v1 = ch1*2.5/4095
            v2 = ch2*2.5/4095
            v3 = ch3*2.5/4095
            v4 = ch4*2.5/4095

            self.adc1.append(v1)
            self.adc2.append(v2)
            self.adc3.append(v3)
            self.adc4.append(v4)

            self.data1.append(ch1)
            self.data2.append(ch2)
            self.data3.append(ch3)
            self.data4.append(ch4)

            if raw[array_size*8+2]!=0xAB:
                print("wave head error")
                break
        self.waves = [[0 for _ in range(15)] for _ in range(4)]
        for i in range(0, 4):
            frame_start = array_size*8+3+i*61

            data_len = raw[frame_start]
            data_start = frame_start+1
            for j in range(data_len):
                cell_start = data_start+j*4
                int_high = raw[cell_start]
                int_low = raw[cell_start+1]
                dec_high = raw[cell_start+2]
                dec_low = raw[cell_start+3]
                get_wave = (int_high<<8)+int_low+((dec_high<<8)+dec_low)*0.001
                # print(i)
                self.waves[i][j] = get_wave
        # print(self.waves)

        self.process_down = True

    def update_plot(self):
        if self.process_down == False:
            return
        self.process_down = False

        x = list(range(array_size))

        self.curve1.setData(x,self.adc1)
        self.curve2.setData(x,self.adc2)
        self.curve3.setData(x,self.adc3)
        self.curve4.setData(x,self.adc4)

        for i in range(4):
            for j in range(15):
                self.num_labels[i][j].setText("{:.3f}".format(self.waves[i][j]) if not self.waves[i][j]==0 else "0")
            if self.visible_lines[i]:
                self.cal_peaks_line(i)
            else:
                for l in self.peaks_lines[i]:
                    l.setVisible(False)

        self.process_down = True

    def cal_peaks_line(self, i):
        datas = [self.data1, self.data2, self.data3, self.data4]
        _data = datas[i]
        if len(_data) == 0:
            return
        ma = max(_data)
        mi = min(_data)
        for j,x in enumerate(_data):
            _data[j] = (x-mi)/(ma-mi)
        peaks = peak_main(_data, statistics.mean(_data))
        print(statistics.mean(_data))

        for l in self.peaks_lines[i]:
            l.setVisible(False)

        for idx,p in enumerate(peaks):
            if p==-1:
                if idx < len(self.peaks_lines[i]):
                    self.plot1.removeItem(self.peaks_lines[i][idx])
                    del self.peaks_lines[i][idx]
                continue
            if idx>=len(self.peaks_lines[i]):
                vline = pg.InfiniteLine(
                    pos=p,
                    angle=90,
                    pen=pg.mkPen('r',width=2,style=QtCore.Qt.DashLine)
                )
                self.plot1.addItem(vline)
                self.peaks_lines[i].append(vline)
            else:
                self.peaks_lines[i][idx].setPos(p)
                self.peaks_lines[i][idx].setVisible(True)

    def toggle_line(self, i, state):
        visible = (state == QtCore.Qt.Checked)
        self.visible_lines[i] = visible

    def toggle_pause(self):
        if self.paused: 
            self.timer.start(1) 
            self.paused = False 
        else: 
            self.timer.stop() 
            self.paused = True

def peak_initial(adc_vec, interval, adc_length, initials_length, threshold):
    initials = [0]*initials_length
    if threshold>0.15:
        return initials
    it = 0
	# for(uint16_t i=1;i<adc_length-1;i++){
    for i in range(1,adc_length-1):
        if it>=initials_length: break
        start = i-interval if i-interval>=0 else 0
        end = i+interval if i+interval<adc_length else adc_length
        front = list(adc_vec)[start:i]
        back = list(adc_vec)[i+1:end]
        if adc_vec[i]>=max(front) and adc_vec[i]>=max(back) and adc_vec[i]>0.6: 
            initials[it] = i
            i += interval
            it+=1
    # print(initials)
    return initials

def peak_main(adc_vec, threshold):
    interval = 50
    adc_length = 3008
    initials_length = 15
	
    initials = peak_initial(adc_vec, interval, adc_length, initials_length, threshold)
    peaks_vec = [-1]*initials_length

    # for(uint8_t i=0;i<10;i++){
    for i in range(initials_length):
        if initials[i]==0:
            break
        start = initials[i]-interval if initials[i]-interval>=0 else 0
        end = initials[i]+interval if initials[i]+interval<adc_length else adc_length
        sumy=0
        sumxy=0
        # for(int j=start;j<end;j++){
        for j in range(start, end):
            sumxy+=adc_vec[j]*j
            sumy+=adc_vec[j]
        peaks_vec[i] = sumxy/sumy

    # print(peaks_vec)
    return peaks_vec

if __name__=="__main__":
    QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling)
    QtWidgets.QApplication.setAttribute(QtCore.Qt.AA_UseHighDpiPixmaps)
    app = QtWidgets.QApplication(sys.argv)
    app.setStyleSheet("""
        QPushButton, QLabel {
            font-size: 12pt;
            font-family: Microsoft YaHei;
        }
    """)
    win = GraphWindow()
    win.setWindowTitle("ADC Plot")
    win.resize(1000,800)
    win.show()
    sys.exit(app.exec_())


