# BOJ 814-2 (18789)
문제 링크 : https://www.acmicpc.net/problem/18789

글꼴 링크 : https://www.jetbrains.com/lp/mono/

코드 블럭 : https://www.fosshub.com/Code-Blocks.html?dwl=codeblocks-20.03mingw-setup.exe


Simulated annealing을 multi thread 환경에서 돌림

한 SA가 끝날 때 마다 Temperature는 올라가고, SA 안에서 Temperature를 점차 감소시키도록 작동시킴

결과가 4000점 중후반대로 계속 수렴해서 8000점 이상을 만들기 위해선 다른 테크닉이 필요해 보인다.



https://blog.naver.com/tlsdydaud1/222540952603
위 링크의 테크닉 중 수열을 이용한 변형, 두 수 교체 를 이용해 5000점 중반대 점수까지 만들어 냈다.
그러나 이전과 마찬가지로 5500점에 강하게 수렴하려는 성질이 생겨 또 다른 테크닉을 이용해 봐야 할 것 같다.
