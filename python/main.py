import numpy as np
import matplotlib.pyplot as plt
import serial
import time

# 1. 초기 변수 및 데이터 리스트 선언
n = 0
intercept = 0
slope = 0
Xlist = []  # 독립변수 (수은 농도 M)
Ylist = []  # 종속변수 (아두이노 측정 전압 V)
x0 = np.ones(5)  # 편향(Bias, 절편) 계산을 위한 상수항 벡터 (1로 채워진 행렬)

# 2. 아두이노 시리얼 통신 설정 (COM3 포트, 보드레이트 9600)
py_serial = serial.Serial(
    port='COM3',
    baudrate=9600,
)

while True:
    # 사용자로부터 기준 농도(Label) 입력받음
    M = float(input("농도:"))
    command = input('a를 눌러 전압 측정:')
    
    # 아두이노로 측정 명령 송신 및 통신 안정화를 위한 대기
    py_serial.write(command.encode())
    time.sleep(1)
    
    # 아두이노로부터 센서 전압 데이터 수신 및 파싱
    if py_serial.readable():
        response = py_serial.readline()
        # 캐리지 리턴 및 라인 피드(\r\n) 제거 후 float 형변환
        V = float(response[:len(response)-2].decode())
        print(f"측정 전압: {V}V")
        
        n = n + 1
        Xlist.append(float(M))
        Ylist.append(float(V))
        print("data", n, "get")
        
        # 3. 5개의 데이터 쌍이 수집되면 머신러닝 학습(선형회귀) 시작
        if n == 5:
            # 수은 농도(Xlist)와 bias(x0)를 결합하여 디자인 행렬(Design Matrix) 생성
            X = np.column_stack([x0, Xlist])
            
            # NumPy의 선형대수 모듈(linalg)의 최소제곱법(lstsq)을 이용하여 가중치 최적화
            # [0] 인덱스를 통해 최소제곱해인 intercept(편향)와 slope(기울기)를 반환받음
            intercept, slope = np.linalg.lstsq(X, Ylist, rcond=None)[0]

            print(f'intercept = {intercept}, slope = {slope}')
            print(f'Fit line: intercept = {intercept:5.2f}, slope = {slope:5.2f}')
            
            # 4. 데이터 시각화 (실측 데이터 산점도 및 최적 회귀선)
            plt.scatter(Xlist, Ylist, color='blue', label='Actual Data') # 원본 데이터는 산점도로 표현
            
            # 수식 계산을 통한 회귀선(Y = slope * X + intercept) 그래프 생성
            y_pred = [intercept + slope * x for x in Xlist]
            plt.plot(Xlist, y_pred, 'r-', label='Linear Regression Line')
            
            plt.xlabel('Mercury Concentration (M)')
            plt.ylabel('Voltage (V)')
            plt.legend()
            plt.show()
            break
