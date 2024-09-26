<h2>환경</h2>

**CMake version > 3.10**

**OpenCV version : opencv-4.10.0-windows**


<h2>프로젝트 생성 방법</h2>

setup.bat 실행




<h2>config 세팅</h2>

config.json 과 exe file은 동일한 path에 위치.

config.json 구조

<code> {
  "load_image_path": "image file name을 포함한 path",
  "save_image_path": "image file을 저장할 path",
  "kernel_width": 자연수,
  "kernel_height": 자연수
}</code>


<h2>결과</h2>

총 3개의 windows가 나타남 ( OpenCV Blurred Image, Custom Blurred Image, Diff Compared Image(diff = RED pixel)


<h3>blurring result windows</h3>

![image](https://github.com/user-attachments/assets/01243959-d166-4277-9ce1-04cacc3b0d73)


두 blur를 비교하여 다른 pixel은 붉게 표시.

<h3>같은 case</h3>

![image](https://github.com/user-attachments/assets/348677e0-99af-4294-95f0-966a9f176f4a)


<h3>다른 case</h3>

![image](https://github.com/user-attachments/assets/968ab748-34ab-4ae6-b19d-6eb23c5120ad)
