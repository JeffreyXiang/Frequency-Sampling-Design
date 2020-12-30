function [H, delta] = linprog_opt(H, nonzeros, N, fc, epsilon)
    nonzeroslen = length(nonzeros);
    len = length(H);
    a = zeros(2*N, nonzeroslen + 1);
    b = zeros(2*N, 1);
    c = zeros(nonzeroslen + 1, 1);
    c(nonzeroslen+1, 1) = 1;
    omega = linspace(0, pi, N);
    
    for i = 0:N-1
        B = H(1) * A_coef(len, omega(i+1), 0) / 2;
        for k = 1:floor(len-1/2)
            if ismember(k, nonzeros) == 0
                B = B + H(k+1) * A_coef(len, omega(i+1), k);
            end
        end
        for k = 0:nonzeroslen-1
            a(2*i+1, k+1) = A_coef(len, omega(i+1), nonzeros(k+1));
            a(2*i+2, k+1) = -A_coef(len, omega(i+1), nonzeros(k+1));
        end
        if (omega(i+1)>fc)
            a(2*i+1,nonzeroslen+1) = -1;
            a(2*i+2,nonzeroslen+1) = -1;
            b(2*i+1, 1) = -B;
            b(2*i+2, 1) = B;
        else
            b(2*i+1, 1) = 1 + epsilon - B;
            b(2*i+2, 1) = -1 + epsilon + B;
        end
    end
    [x, delta] = linprog(c, a, b);
    for k = 0:nonzeroslen-1
        H(nonzeros(k+1)+1) = x(k+1);
    end
    H = linearPhase(H);
end
