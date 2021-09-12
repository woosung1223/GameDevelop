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


#### 개발 과정
##### 1. 공의 개수가 많아지면 FPS가 낮아지는 문제점 발생 
---
Item 중 공의 개수를 두 배로 늘려주는 아이템이 존재함. 다만, 아이템을 계속 사용하면서 공의 개수를 늘려가다 보니, 게임의 FPS가 점점 늦어지는 것을 확인함. 
이를 해결하기 위해서 여러 방법을 찾다가 멀티 스레드 방식을 알게 됨. 예전에는 CPU가 1개의 코어를 가졌는데, 이 경우 한번에 한 개의 연산만 수행이 가능하기 때문에 아래와 같이 
컨텍스트 스위칭(Context switching) 방식을 사용. 하나의 코어에서 여러 개의 프로그램을 분할시켜서 동시에 진행되는 것처럼 보이게 하는 방법.

이 CPU 코어에서 돌아가는 프로그램 단위를 쓰레드(thread) 라고 함. 즉 CPU 코어 하나에서는 한 번에 한 개의 쓰레드만 실행시킬 수 있고, 여러 개의 쓰레드로 구성된 프로그램을 멀티 쓰레드 프로그램이라고 함. 

따라서 공마다 쓰레드를 할당해서 멀티 쓰레드 프로세싱이 가능하지 않을까 생각했음. 객체간의 기준이 뚜렷하고, 각각의 공이 상호작용 하는 부분이 없기에 병렬화가 가능. 데이터가 dependent한 경우, 병렬화의 의미가 없어지기 때문에 멀티 쓰레드 방식을 사용하기 어려움.
![image](https://user-images.githubusercontent.com/78679830/132990509-53d5ae1d-b870-446e-8462-a19a06634454.png)

멀티 쓰레드 방식을 적용하기 전에, 공이 많아지면 왜 느려질까에 대해 생각해봄. main loop에서 ball 객체가 많아짐에 따라 영향을 받을 수 있는 부분은 block 객체와의 상호작용 부분. 아래는 공과 블록이 충돌했는지를 확인하는 코드이다.
```C++
void CheckCollision(BallHandler& ballhandler) {
 for (auto& block : Blocks) {
	if (block != nullptr && !block->deleting) { // 삭제된 블록이 아니거나 삭제될 예정인 블록이 아닌 경우에만 
			for (auto& ball : ballhandler.balls) {
			if (ball != nullptr && (block->pos - ball->pos).getMagnitude() < 0.1f) {
			vec2 left[2] = { vec2(block->pos.x - 0.02f, block->pos.y - 0.015f), vec2(block->pos.x - 0.015f, block->pos.y + 0.015f) };
			vec2 right[2] = { vec2(block->pos.x + 0.015f, block->pos.y - 0.015f),vec2(block->pos.x + 0.02f, block->pos.y + 0.015f) };
			vec2 down[2] = { vec2(block->pos.x - 0.015f, block->pos.y - 0.02f), vec2(block->pos.x + 0.015f, block->pos.y - 0.015f) };
			vec2 up[2] = { vec2(block->pos.x - 0.015f, block->pos.y + 0.015f), vec2(block->pos.x + 0.015f, block->pos.y + 0.02f) };
			vec2 inside[2] = { vec2(block->pos.x - 0.0075f, block->pos.y - 0.0075f), vec2(block->pos.x + 0.0075f, block->pos.y + 0.0075f) };
			// boundary check를 위한 space 생성
			// 모서리방향 4방향 부딪혔을 때 -> 블록 안에 들어왔는지.
			// 왼쪽 벽
			// 모서리에 충돌 시 ball->vel을 1도정도 회전시킴. 무한하게 왔다갔다하는 것을 방지하기 위함. 
			if (ball->pos + vec2(ball->radius, 0.0f) >= left[0] && ball->pos + vec2(ball->radius, 0.0f) <= left[1]) {
				if(!block->rigid)
					block->life -= 1;

				if (ball->vel.x >= 0.0f)
					ball->vel.x *= -1.0f * coef_res;
			}

			// 오른쪽 벽
			else if (ball->pos - vec2(ball->radius, 0.0f) >= right[0] && ball->pos - vec2(ball->radius, 0.0f) <= right[1]) {
				if (!block->rigid)
 				block->life -= 1;

				if (ball->vel.x <= 0.0f)
					ball->vel.x *= -1.0f * coef_res;
}
```
위와 같이 n^2의 시간 복잡도를 가지기 때문에, ball 객체가 커질수록 점점 FPS가 내려가는 문제점이 발생한 것. 따라서 코드 일부 수정.
```C++
 if(ball != nullptr && (block->pos - ball->pos).getMagnitude() < 0.1f) 
```
위 부분에서 AND operator 뒷부분이 추가한 부분이다. 이전까지는 뒷부분이 추가되어 있지 않았기 때문에, 모든 블록에 대해 공과 충돌했는지 판단하였기 때문에 시스템이 느려진 것. 따라서 위처럼 공과 블록이 인접했을 때에만 충돌을 검사하도록 수정하였음. 

멀티쓰레드 방식보다는 충돌조건을 바꾸는 방식이 비용적 측면에서 싸기 때문에 이 방법을 채택함으로써 문제 해결.

