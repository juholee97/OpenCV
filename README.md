<h2>환경</h2>

**CMake version > 3.10**

**OpenCV version : opencv-4.10.0-windows**


<h2>프로젝트 생성 방법</h2>

setup.bat 실행




<h2>config 세팅</h2>

config.json 과 exe file은 동일한 path에 위치.

config.json 구조

<code> {
  "load_image_path": "image file이 위치한 path",
  "save_image_path": "image file을 저장할 path",
  "kernel_width": 자연수,
  "kernel_height": 자연수
}</code>


<h2>결과</h2>

총 １개의 windows가 나타남 ( Diff Compared Image(diff = RED pixel) ）

두 blur를 비교하여 다른 pixel은 붉게 표시하며，다른 이미지는 이미지 하단에 픽셀 수를 출력함．

<h3>같은 case</h3>
![image](https://github.com/user-attachments/assets/c6c55c16-79f5-4850-a0ba-a469dc4b11be)


<h2>동작 성능</h2>

<h3>속도</h3>

![image](https://github.com/user-attachments/assets/30784cab-e8a5-449d-b4a4-52e157a390f9)
BlurCustom (Separable Convolution로 구현)

<h3>메모리 사용량</h3>

BlurCustom 
![image](https://github.com/user-attachments/assets/41eb3aca-1c3c-4222-b3cc-c68c6b900467)

BlurOpenCV
![image](https://github.com/user-attachments/assets/b61a166a-f668-4a9b-a772-13a3eca1f9b5)

BlurOpenCV의 메모리 사용량과 성능은 buffer ＞ copied_buffer ＞ inputImage ＞ outputImage ＞ blurred_buffer 순서로 동작하며 효율적이지 않게 측정된 것으로 보임．

