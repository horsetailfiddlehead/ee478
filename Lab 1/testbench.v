module testBench;
  wire disc, inc, WE, store, OE, valid, readNotWrite, clk, rst;
  
  SRAMDriver myDriver(disc, inc, WE, store, OE, valid, readNotWrite, clk, rst);
  
  myTest tester(readNotWrite, clk, rst, disc, inc, WE, store, OE, valid);
  
endmodule

module myTest(readNotWrite, clk, rst, disc, inc, WE, store, OE, valid);
  output clk, rst, readNotWrite;
  
  input disc, inc, WE, store, OE, valid;
  
  reg clk, rst, readNotWrite;
  wire[0:5] outputBus = {disc, inc, WE, store, OE, valid};
  parameter period = 5;
  parameter stimDelay = 20;
  
  initial begin
    clk = 0;
    rst = 1;
    readNotWrite = 1;
  end
    
  always begin
    #period clk = ~clk;
  end
    
  always @(posedge clk);
  initial begin
    #period rst = 0;
    #(stimDelay*5);
    rst = 1;
    readNotWrite = 0;
    #stimDelay rst = 0;
    #(stimDelay*5);
    $finish;
  end
endmodule