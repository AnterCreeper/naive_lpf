# Naive Low Pass Filter Algorithm

### Overview

A Naive LPF, cost ultra-tiny resources.   
Less than 100 lines to save 1$ (pcmd3140, adau1002, etc..)

### Usage:
assume result is target and dmic_clk and dmic_din is input IO.
```
reg[15:0] result;

wire mic_vld;
wire[15:0] mic_dat;
dmic mic(
    .clk(dmic_clk),
    .rst_n(!rst),
    .din(dmic_din),
    .valid(mic_vld),
    .dout(mic_dat)
);

reg vld0, vld1;
reg[15:0] dat0, dat1;
always @(posedge iclk or posedge rst)
begin
    if(rst)
    begin
        vld0 <= 0;
        vld1 <= 0;
    end else
    begin
        vld0 <= mic_vld;
        vld1 <= vld0;
        dat0 <= mic_dat;
        dat1 <= dat0;
        if(vld1 && vld0)
        begin
            result <= dat1;
        end
    end
end
```
