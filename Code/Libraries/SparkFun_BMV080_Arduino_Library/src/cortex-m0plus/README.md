# Install the BOSCH BMV080 SDK library files for this folder

### From the Bosch BMV080 SDK, copy the following files to this folder.

Using a shell:

Where BOSCH_SDK = *The install directory of the downloaded Bosch SDK*

```sh
cp $BOSCH_SDK/arm_cortex_m0plus/arm_none_eabi_gcc/release/lib_bmv080.a   lib_bmv080.a
cp $BOSCH_SDK/arm_cortex_m0plus/arm_none_eabi_gcc/release/lib_postProcessor.a   lib_postProcessor.a
```

Or just drag and drop via a GUI interface.
