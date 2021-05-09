# PDS

## Project

- 25 bodů
- do 25. 4. 2021 23:59

<!----------------------------------------------------------------------------->

### Links

> [**Assignment**](assignment/assignment.pdf)<br>
> — Description of the assignment in Czech

> [**BUT FIT Wiki**](https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs?id=14110&csid=735727)<br>
> — Wiki page of the subject

<!----------------------------------------------------------------------------->

### Description

- Z hlaviček odchyceného šifrovaného provozu (TLS) rozpoznat o jakou se jedná aplikaci.
- Evaluate how mobile encrypted traffic can be identified using TLS
  fingerprinting.
- TLS fingerprinting method
  - JA3

<!----------------------------------------------------------------------------->

### Software versions

- Pixel 4XL, Android 11, x86
- Yr 5.13.2 (nodpi)
- Settle Up: Group Expenses 10.0.2030
- Forest: Stay focused 4.35.1
- Forza Football: Live soccer scores 5.1.13
- Revolut 7.43.1
- GitHub 1.7.4 (nodpi)
- Netflix 7.97.1
- Twitter 8.88.0
- YouTube 16.12.34
- Phoenix 1.4.8

<!----------------------------------------------------------------------------->

### Commands

```
$ emulator -list-avds
```

```
$ emulator -avd Pixel-4XL_Android-11_x86 -port 12345
```

```
$ adb devices -l
```

```
$ adb -s emulator-12345 shell monkey -p se.footballaddicts.livescore -v 1000
```

```
com.revolut.revolut
se.footballaddicts.livescore
com.google.android.youtube
no.nrk.yr
com.twitter.android
fr.acinq.phoenix.mainnet
com.github.android
cc.forestapp
com.netflix.mediaclient
cz.destil.settleup
```

<!----------------------------------------------------------------------------->

### Resources

- IANA TLS parameters
  - https://www.iana.org/assignments/tls-parameters/tls-parameters.xhtml
- IANA TLS extensions
  - https://www.iana.org/assignments/tls-extensiontype-values/tls-extensiontype-values.xhtml
