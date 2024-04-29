function createMIF(infilepath, width, fdepth, rowcount, colcount, scale, ...
    fname)
% Converts matrix into .mif format for loading into ROM
    datastruct = load(infilepath);
    datamat = datastruct.dmat .* scale;

    % Define data format for components of delay coefficients
    % Ex: depth = 28, fdepth = 4 --> [10b.4b | 10b.4b]
    q = quantizer([width/2, fdepth]);

    header = sprintf('WIDTH=%d;\nDEPTH=%d;\n', width, rowcount*colcount);
    header = header + "ADDRESS_RADIX=HEX;" + newline + "DATA_RADIX=BIN;" + newline;
    writelines(header, fname);
    writelines("CONTENT", fname, WriteMode="append");
    writelines("BEGIN", fname, WriteMode="append");

    % Delay ROM Structure
    % [  B1  ][  B2  ][  B3  ][  B4  ]
    % [M1..M4][M1..M4][M1..M4][M1..M4]
    % Matrix Structure = 4 x 13 (mics x directions)
    dims = size(datamat);
    for bnum = 1:dims(2)
        for mnum = 1:dims(1)
            addr = dec2hex((bnum-1)*dims(1) + (mnum-1), 2);
            val = datamat(mnum, bnum);
            binval_a = num2bin(q, real(val));
            binval_b = num2bin(q, imag(val));
            binval = strcat(binval_a, binval_b);
            nextline = sprintf(' %s : %s;', addr, binval);
            writelines(nextline, fname, Writemode="append");
        end
    end

    writelines("END;", fname, WriteMode="append");
end