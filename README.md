silk
======

A library for encoding and decoding silk file,support Java and Kotlin.


Usage
-----

- Encode pcm file into silk file

```kotlin

SilkEncoder.doEncode(pcmFile.absolutePath, slkFile.absolutePath)
```

- Decode silk file into pam file

```kotlin

SilkDecoder.doDecode(slkFile.absolutePath, pcmFile.absolutePath)
```
