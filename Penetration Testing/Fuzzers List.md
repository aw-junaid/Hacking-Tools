
# Awesome Fuzzers List

A curated list of fuzzing tools, frameworks, and resources categorized by fuzzing type, platform, and purpose.

## Types of Fuzzers

- **Dumb Fuzzer**: No knowledge of data structure, generates random inputs.
    
    - Examples: [Radamsa](https://gitlab.com/akihe/radamsa)
- **Generation Fuzzer**: Generates inputs based on specifications or formats.
    
    - Examples: [Peach](https://wiki.mozilla.org/Security/Fuzzing/Peach), [Sulley](https://github.com/OpenRCE/sulley), [Boofuzz](https://github.com/jtpereyda/boofuzz)
- **Coverage Guided Fuzzer**: Monitors code coverage and program flow with instrumentation.
    
    - Examples: [Jazzer](https://github.com/CodeIntelligenceTesting/jazzer), [AFL++](https://github.com/AFLplusplus/AFLplusplus)

---

## Dictionary-based Fuzzers

- [p0dalirius HTTP Fuzzing Scripts](https://github.com/p0dalirius/http-fuzzing-scripts/tree/master)
- [RAFT Wordlists](https://github.com/danielmiessler/SecLists/tree/master/Discovery/Web-Content/raft)
- [OpenDoor](https://github.com/stanislav-web/OpenDoor)

---

## Web Fuzzers

- [LFI Fuzzer for Burp Suite](https://github.com/luke-goddard/LFI-Fuzzer-Burp-Suite)
- [Burp Suite Extensions](https://github.com/snoopysecurity/awesome-burp-extensions)
- [FeroxFuzz](https://github.com/epi052/feroxfuzz)
- [Recollapse](https://github.com/0xacb/recollapse)

---

## Network Fuzzers

- [Peach](https://wiki.mozilla.org/Security/Fuzzing/Peach)
- [MedAudit](https://github.com/anirudhduggal/medaudit)
- [Fuzzotron](https://github.com/denandz/fuzzotron)
- [Sulley](https://github.com/OpenRCE/sulley)
- [Boofuzz](https://github.com/jtpereyda/boofuzz)

---

## All-in-One Resources

- [The Art of Fuzzing](https://github.com/raminfp/the-art-of-fuzzing)
- [Fuzzing 101](https://github.com/antonio-morales/Fuzzing101)

---

## Fuzzing Frameworks

- **Executable Simulation**: [Qiling Framework](https://github.com/qilingframework/qiling)
- **Extension Fuzzer**: [FormatFuzzer](https://github.com/uds-se/FormatFuzzer)
- **eBPF Fuzzer**: [Buzzer](https://github.com/google/buzzer)

---

## Benchmarks and Vulnerable Programs

- **Benchmark Fuzzers**: [Fuzzbench](https://github.com/google/fuzzbench)
- **Vulnerable Programs**: [Damn Vulnerable C Program](https://github.com/hardik05/Damn_Vulnerable_C_Program)

---

## Windows Fuzzing

- [WTF Snapshot-based Fuzzer](https://github.com/0vercl0k/wtf)
- [WinAFL](https://github.com/googleprojectzero/winafl)

---

## Linux Fuzzing

- [AFL++](https://github.com/AFLplusplus/AFLplusplus)
- [NEUZZ](https://github.com/dongdongshe/neuzz)
- [Libfuzzer](https://llvm.org/docs/LibFuzzer.html)

---

## Cross-platform Fuzzing

- [kAFL](https://github.com/IntelLabs/kAFL)
- [Radamsa](https://gitlab.com/akihe/radamsa)

---

## Specialized Fuzzing

- **CPU Fuzzing**: [Silifuzz](https://github.com/google/silifuzz)
- **Continuous Fuzzing**: [OSS-Fuzz](https://github.com/google/oss-fuzz)
- **Precompiled Fuzzers**: [Fuzztest](https://github.com/google/fuzztest), [MemorySanitizer](https://github.com/google/sanitizers/wiki/MemorySanitizer)

---

## Language-Specific Fuzzers

- **Go**: [go-fuzz](https://github.com/dvyukov/go-fuzz)
- **Rust**: [cargo-fuzz](https://github.com/rust-fuzz/cargo-fuzz)
- **JavaScript**: [Jazzer.js](https://github.com/CodeIntelligenceTesting/jazzer.js), [Fuzzilli](https://github.com/googleprojectzero/fuzzilli)
- **Python**: [Atheris](https://github.com/google/atheris)
- **Java**: [Jazzer](https://github.com/CodeIntelligenceTesting/jazzer)

---

## Platform-Specific Fuzzing

- **Android Fuzzing**: [Android Fuzzing](https://github.com/quarkslab/android-fuzzing)
- **Samsung & MediaTek Fuzzing**: [FirmWire](https://github.com/FirmWire/FirmWire)

---

## Browser Fuzzing

- [Dharma](https://github.com/posidron/dharma)
- [Freedom](https://github.com/sslab-gatech/freedom)
- [BFuzz](https://github.com/RootUp/BFuzz)
- [SaGe Browser Fuzzer](https://github.com/ChijinZ/SaGe-Browser-Fuzzer)

---

## Blockchain Fuzzing

- [Beaconfuzz Presentation](https://fuzzinglabs.com/wp-content/uploads/2022/02/Beaconfuzz_OffensiveCon2022_patrick_ventuzelo.pdf)

---

### Books and Learning Resources

- **Books**: _Fuzzing Against the Machine_, _Gray Hat Python_
- **Build Custom Fuzzers**: [Fuzzing Book](https://www.fuzzingbook.org/)
