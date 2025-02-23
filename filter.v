module iter(
    input clk,
    input rst_n,
    input[20:0] din,
    output reg[20:0] dout
);

wire[21:0] sub;
assign sub = {din[20],din} - {dout[20],dout};

always @(posedge clk or negedge rst_n)
begin
    if(!rst_n)
    begin
        dout <= 0;
    end else
    begin
        dout <= {{5{sub[21]}},sub[20:5]} + dout;
    end
end

endmodule

module dmic(
    input clk,
    input rst_n,
    input din,
    output valid,
    output[15:0] dout,
    output[7:0] dbg
);

reg data;
always @(posedge clk)
begin
    data <= din;
end

wire[20:0] dext[0:4];
assign dext[0] = {~data,{15{data}},5'b0};
genvar i;
generate
for(i = 0; i < 4; i = i + 1)
begin
iter pass(
    .clk(clk),
    .rst_n(rst_n),
    .din(dext[i]),
    .dout(dext[i+1])
);
end
endgenerate

wire[15:0] pcm;
assign pcm = dext[4][20:5];
reg[5:0] cnt;
reg[21:0] acc;

assign valid = &(~cnt);
assign dout = acc[21:6];

assign dbg = 0;

always @(posedge clk or negedge rst_n)
begin
    if(!rst_n)
    begin
        cnt <= 0;
    end else
    begin
        cnt <= cnt + 1;
        acc <= valid ? {{6{pcm[15]}}, pcm} : acc + {{6{pcm[15]}}, pcm};
    end
end

endmodule
