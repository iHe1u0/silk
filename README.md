
![license](https://img.shields.io/aur/license/android-studio)
![Release Version](https://img.shields.io/badge/release-1.0.0-brightgreen.svg)
![Platform](https://img.shields.io/badge/Platform-%20Android-brightgreen.svg)


# silk

A library for encoding and decoding silk file,support Java and Kotlin.

## Feature

* Encode pcm with silk
* Decode silk to pcm

## Getting Started

### Installation Via Maven
Add the following lines to `build.gradle` on your app module:

```gradle
dependencies {
    implementation 'cc.imorning:silk:1.0.0'
    // replace "1.0.0" with any available version
}
```

### Quick Tutorial

- Encode pcm file into silk file

```kotlin
MainScope().launch(Dispatchers.IO) {
    SilkEncoder.doEncode(pcmFile.absolutePath, slkFile.absolutePath).orEmpty()
}
```

- Decode silk file into pcm file

```kotlin
MainScope().launch(Dispatchers.IO) {
    SilkDecoder.doDecode(slkFile.absolutePath, pcmFile.absolutePath).orEmpty()
}
```

## FAQ & Feedback

Post an [issues](https://github.com/morningos/silk/issues) if you encounter any problem in using.

## License

```
Copyright 2023 iMorning

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
