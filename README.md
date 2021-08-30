## 2021 Summer) OpenGL 을 이용한 C++ Bouncing Ball 게임 개발
 ------
#### 프로젝트 계획이유
> c++와 자료구조를 배운 시점에서 내가 할 수 있는 사이드 프로젝트를 기획하고 싶었다. 이전까지는 내가 무엇인가를 직접 만들어 본적이 없었고, 만들었다고 하더라도 콘솔창에서 실행되는 프로그램이 대다수였다. 따라서 이번 여름에는 게임 개발에 부딪혀 보기로 했다.

> 목표는 무언가 결과물을 도출함과 동시에 그 과정에서 객체지향을 정확히 이해하는 것. 실질적인 개발이 처음이기에 실패하더라도 과정에서 많은 것을 얻을 수 있을 것 같았다. 

#### 사용한 라이브러리
> OpenGL, FMOD


#### 게임 플레이 화면 
<img src = "https://user-images.githubusercontent.com/78679830/131140869-f64d43d5-f209-402c-8c7e-0d6ead574d2f.PNG" width = "20%" height = "20%"> <img src = "https://user-images.githubusercontent.com/78679830/131141316-eef9dbba-1a84-4f07-8794-9d1946a551a9.PNG" width = "20%" height = "20%"> <img src = "https://user-images.githubusercontent.com/78679830/131141160-f67ed3a0-afe3-4a40-96af-7598fb4c176c.PNG" width = "20%" height = "15%"> <img src = "https://user-images.githubusercontent.com/78679830/131292045-f5fa1dd1-2a6d-44cd-a2ce-73c95bdd5582.PNG" width = "20%" height = "20%">

 
         게임 시작화면            게임 플레이화면            게임 종료화면(실패)
      게임 종료화면(클리어)
                
> 크게는 게임 페이지를 세 분류로 나눴다. 게임 시작화면에서 start 버튼을 누르면 게임 플레이화면으로 이동하고, exit 버튼을 누르면 게임이 종료된다.
> 
> 게임 플레이화면에서는 공이 전부 화면 밖으로 벗어나면 게임 종료화면으로 이동하게 된다.
> 
> 게임 종료화면에서 ESC 버튼을 눌러 게임이 종료될 수 있도록 했다.

#### 구현 요소
* 아이템 
* 블록 
* 공
* 블록, 공, 아이템 상호작용
* 배경 음악
* 키보드 입력 상호작용
* 인터페이스

#### 디자인 패턴
<img src = "https://user-images.githubusercontent.com/78679830/131291189-dc1ab423-a865-48dd-8719-b6c4647664e2.PNG">

> 사운드 엔진은 SINGLETON 패턴을 사용하였음.

#### 실행 순서도(flow chart)
![flowchart](https://user-images.githubusercontent.com/78679830/131296750-4c3ad1b6-911d-454e-83f7-da3e2ac335ab.PNG)
