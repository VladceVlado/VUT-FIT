# BUILD_TYPE in {debug|release}
BUILD_TYPE=release
USERNAME=xlogin00
PASSWORD=password

.PHONY: build run clean

build:
	bash gradlew build

run:
	bash gradlew run -Dbuild_type=$(BUILD_TYPE) -Dusername=$(USERNAME) -Dpassword=$(PASSWORD)

clean:
	bash gradlew clean
	rm -rf .user-data-* .gradle/ xdusek27.zip

pack:
	@zip -r xdusek27 build.gradle  db  docs .gitignore  gradle  gradle.properties  gradlew LICENSE  Makefile  README.md  settings.gradle  src
