# socket2018YB
> 소스 코드 관리 규칙
>> 1. master는 교수만 업데이트한다.
>> 2. 교수는 수업 시간에 주어진 실습 문제 풀이를 수업 다음 날까지 master 브랜치에 업데이트한다.
>> 3. 학생은 각자의 이름으로 브랜치를 생성하고, 자신 브랜치에서만 작업한다. 
* * *
### 1. github.com에 회원 가입 후 로그인 수행
### 2. socket2018YB 소스코드 다운로드 (git clone https://github.com/hk-park/socket2018YB.git )
> 1) 다운로드가 완료되면, socket2018YB 디렉토리가 생성. 
> 2) socket2018YB로 이동
* * *
### 3. 자신의 소스코드 관리를 위한 브랜치 생성 (최초 한 번만 수행)
> 1) **git branch "자신의이름"** (예를 들어, git branch 홍길동)
>> * 자신의 소스코드를 관리할 새로운 브랜치를 생성함. 자신의 이름이 브랜치의 이름이 됨.
>> * 브랜치의 이름은 기본적으로 한글 이름으로 작성함. 
> 2) **git checkout "자신의이름"** (예를 들어, git checkout 홍길동)
>> * 자신의 브랜치로 이동
> 3) **git push origin "브랜치명"**
>> * 자신의 브랜치를 원격으로 업로드
>> * 현재 master 브랜치에 있는 소스코드를 그대로 업로드한 것임. 

>>> 만약 자신의 소스코드로 브랜치를 생성하고 싶은 경우에는? 
>>>>> * 위의 1,2번 수행
>>>>> * 모든 파일 삭제(rm -rf  *)
>>>>> * 자신의 소스코드 파일을 저장
>>>>> * git add *  수행
>>>>> * git commit -m "나의 첫번째 소스코드" 수행
>>>>> * git push origin "자신의이름" 수행
* * *
### 4. 매주 실습 내용 업데이트 방법 (매주 수업 끝나고 수행)
> 1) **[수업 전] 실습 환경 세팅**
>> * socket2018YB 소스코드 다운로드 (git clone https://github.com/hk-park/socket2018YB.git)
>> * socket2018YB 디렉토리로 이동
>> * git checkout -b "자신의이름" origin/"자신의이름" (예를 들어, git checkout -b 홍길동 origin/홍길동)
>>> - 여기까지 하면, 지난 주에 자신이 업로드했던 소스코드를 볼 수 있다.
>>> - git branch 명령어를 수행하여, 자신의이름으로 된 브랜치명 옆에 * 표시가 있고, 녹색 글씨로 표기가 되어 있는지 꼭 확인!
> 2) **[수업 도중] 각자 실습 내용 구현**
> 3) **[수업 후]**
>> + A. **git status** 수행 (마지막 수정 이후에 어떤 파일이 변경되었는지 보여줌.)
>> + B. **git add "파일명"** 수행 
>>> - "파일명"은 조금 전에 git status 수행한 결과 중에 빨간색으로 쓰여져 있는 파일명(Untracked Files)을 의미함.
>>> - 파일이 여러 개면 여러 번 수행 
>> + C. **git commit -m "커밋 메세지"** 수행
>> 커밋 메세지는 아래 규칙에 맞춰서 작성한다.
>>> * 모든 실습 예제를 수행하였으면, [수업날짜] 모든 실습 완료 (예를 들어, git commit -m "[10.04] 모든 실습 완료")
>>> * 일부만 수행하였으면, [수업날짜] 구현한 실습 예제번호까지 완료(예를 들어, git commit -m "[10.04] 5-2까지 완료")
>>> * 모두 수행못하였으면, [수업날짜] 미완료 (예를 들어, git commit -m "[10.04] 미완료")
>> + D. **git push origin "자신의이름"** 수행 (예를 들어, git push origin 홍길동)
