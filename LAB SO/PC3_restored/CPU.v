module CPU(clock50, clock, switch, resetCPU, a, b, c, pushbutton, HDIndata, HDoutdata, HDwe, HDaddress, BIOSInst, address, Signal, saidajump, instrucao);

	input resetCPU, clock50, pushbutton;
	input[16:0] switch;
	input[31:0] HDIndata, BIOSInst;
	output HDwe, clock;
	output[6:0] a, b, c;
	output[31:0] HDoutdata, saidajump, instrucao;
	output [9:0] address;
	output[11:0] HDaddress;
	output[1:0] Signal;
	
	wire[31:0] addres_new, s1, s2, saida_extensor, saidamux2, saidamux3beq, saidaULA, saidamemoria, saidamux3, data; //saidajump = RD, s1= R1, s2 = R2
	wire[31:0]  RegHDdata, memInst;
	wire[1:0] beqjump, flag;
	wire igual, out, button, rBIOS, clockReg, savePC, CS;
	wire[1:0] sel;
	wire[3:0] oper;
	wire writereg, writemem, rjal, smuxjump, smux2beq, jump, ls, selectPC, in, smuxdm, smuxreg, smuxHD, writeIM, wreReg, smuxAdDM, CSe;
	wire[1:0] smux3, smux3beq, controlPCUC, controlPCIO, controlPC;
	wire[9:0] addressDM;
	
	// Divisor de frequencia
	DivisorF divisor(.clk(clock50), .s(clock));
	
	//Debouncer
	debounce debounce(.button(pushbutton), .clock(clock), .saida(button));
	
	//Multiplexador para a saída do HD
	mux2 muxoutHD(.a(saidamemoria), .b(RegHDdata), .escolhe(smuxHD), .saida(HDoutdata));
	
	//Modulo de interrupcao
	Interruption Interruption(.in(in), .out(out), .controlIO(button), .controlPC(controlPCIO), .inst(instrucao[31:26]), .CSe(CSe), .clock(clock), .clock50(clock50), .savePC(savePC), .reset(resetCPU), .CS(CS));
	
	//Controle do PC para a interrupcao
	mux21v muxcontrolPC(.a(controlPCUC), .b(controlPCIO), .escolhe(selectPC), .saida(controlPC));
	
	//Contador de programa
	PC PC(.endereco(address), .clock(clock), .flag(flag), .novoendereco(addres_new), .reset(resetCPU), .resetBIOS(rBIOS), .CSe(CSe));
	
	//Memoria de instrucao
	memoriainstrucao memoriainstrucao (.data(HDIndata), .read_addr(address), .write_addr(saidajump[9:0]), .we(writeIM), .read_clock(clock50), .write_clock(clock), .q(memInst));
	
	//Multiplexador de mem inst e BIOS
	biosmux biosmux(.clock(clock) ,.reset(resetCPU) ,.resetPC(rBIOS) ,.biosInst(BIOSInst) ,.memInst(memInst) ,.out(instrucao));
	
	//Unidade de controle
	UC UC(.op(instrucao[31:26]), .sel(sel), .oper(oper), .writereg(writereg), .writemem(writemem), .rjal(rjal), .smuxjump(smuxjump), .smux2beq(smux2beq), .out(out), .jump(jump), .smux3(smux3), .smux3beq(smux3beq), .controlPC(controlPCUC), .ls(ls), .selectPC(selectPC), .in(in), .smuxdm(smuxdm), .smuxreg(smuxreg), .smuxHD(smuxHD), .writeIM(writeIM), .HDwe(HDwe), .wreReg(wreReg), .smuxAdDM(smuxAdDM), .signalLCD(Signal));
	
	//Multiplexadores
	mux3 mux3beq(.a(s2), .b(saida_extensor), .c(s1), .escolhe(smux3beq), .saida(saidamux3beq));
	mux2 muxjump(.a(saidajump), .b(saida_extensor), .escolhe(smuxjump), .saida(addres_new));
	mux2 mux2beq(.a(s1), .b(saidajump), .escolhe(smux2beq), .saida(saidamux2));
	mux2 muxDM(.a(s1), .b(HDIndata), .escolhe(smuxdm), .saida(data));
	mux4 mux4(.a(saidamemoria), .b(saida_extensor), .c(saidaULA), .d(s1), .escolhe(smux3), .saida(saidamux3));
	muxPC muxPC(.a(2'b00), .b(beqjump), .c(2'b10), .escolhe(controlPC), .saida(flag));
	mux2 muxDMaddress(.a(saidaULA[9:0]), .b(saidajump[9:0]), .escolhe(smuxAdDM), .saida(addressDM));
	
	//Banco de registradores
	bancoreg bancoreg(.escrita(writereg), .rjal(rjal), .rd(instrucao[25:21]), .r1(instrucao[20:16]), .r2(instrucao[15:11]), .clock(clock), .write(saidamux3), .PC(address), .s1(s1), .s2(s2), .jump(saidajump), .wrenHD(wreReg), .writeHD(HDIndata), .outHD(RegHDdata), .savePC(savePC));//.resetCPU(resetCPU), .a(a),.b(b),.c(c));
	
	//Extendor de bits
	extensorbit extensorbit(.entrada1(instrucao[15:0]), .entrada2(instrucao[20:0]), .entrada3(switch[15:0]), .selecao(sel), .saida(saida_extensor));
	
	//ULA
	ULA ULA(.dado1(saidamux2), .dado2(saidamux3beq), .operacao(oper), .saida(saidaULA), .igual(igual));
	
	//Calcula endereco
	calculaendereco calculaendereco(.s1(s1[11:0]), .s2(s2[11:0]), .saida(HDaddress));
	
	//Memoria de dados
	memoriadados memoriadados (.data(data), .addr(addressDM), .we(writemem), .clk(clock), .clk50(clock50), .q(saidamemoria));
	
	//Modulo de saida
	saida saida(.out(out), .numero(saidajump), .res(resetCPU), .saida1(a), .saida2(b), .saida3(c), .clock(clock));
	
	//Or
	beqorjump beqorjump(.a(jump), .b(igual), .flag(beqjump));
	
	
endmodule