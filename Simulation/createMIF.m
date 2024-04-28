function createMIF(infilepath, depth, fdepth, rowcount, colcount, fname)
% Converts matrix into .mif format for loading into ROM
    datastruct = load(infilepath);
    datamat = datastruct.dmat;

    % Define data format for components of delay coefficients
    % Ex: depth = 28, fdepth = 4 --> [10b.4b | 10b.4b]
    q = quantizer([depth/2, fdepth]);

    header = sprintf('WIDTH=%d;\nDEPTH=%d;\n', rowcount*colcount, depth);
    header = header + "ADDRESS_RADIX=HEX;" + newline + "DATA_RADIX=BIN;" + newline;
    writelines(header, fname);
    writelines("CONTENT BEGIN", fname, WriteMode="append");

    dims = size(datamat);
    for row = 1:dims(1)
        for col = 1:dims(2)
            addr = dec2hex((row-1)*dims(2) + (col-1), 2);
            val = datamat(row, col);
            hexval_a = num2bin(q, real(val));
            hexval_b = num2bin(q, imag(val));
            hexval = strcat(hexval_a, hexval_b);
            nextline = sprintf('\t[%s]\t:\t%s;', addr, hexval);
            writelines(nextline, fname, Writemode="append");
        end
    end

    writelines("END;", fname, WriteMode="append");
end