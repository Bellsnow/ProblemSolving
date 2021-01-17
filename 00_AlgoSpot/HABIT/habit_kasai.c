#include <stdio.h>

#define MAX_S   (4000)

char str[MAX_S+1];
int SA[MAX_S];
int LCP[MAX_S];
int group[MAX_S+1];
int K;

int strLen(const char* str) {
    int len = 0;
    while(*str++) len++;
    return len;
}

int comp(int a, int b, int size) {
    if(group[a] == group[b])
        return group[a + size] < group[b + size];
    return group[a] < group[b];
}

void sortSA(int s, int e, const int size) {
    int m = (s + e) >> 1;
    int i, j, k;
    int tmpSA[MAX_S + 1];

    if(s >= e) return;

    sortSA(s, m, size);
    sortSA(m+1, e, size);

    i = k = s; j = m + 1;
    while(i <= m && j <= e) {
        if(comp(SA[i], SA[j], size))
            tmpSA[k++] = SA[i++];
        else
            tmpSA[k++] = SA[j++];
    }
    while(i <= m) tmpSA[k++] = SA[i++];
    while(j <= e) tmpSA[k++] = SA[j++];

    for(k = s; k <= e; k++)
        SA[k] = tmpSA[k];
}

void makeSuffixArray(int len) {
    int t; int newGroup[MAX_S + 1];
    for(int i = 0; i < len; i++) {
        group[i] = str[i] - 'a';
        SA[i] = i;
    }
    t = 1; group[len] = -1;
    while(t < len) {
        sortSA(0, len-1, t);
        newGroup[SA[0]] = 0;
        for(int i = 1; i < len; i++) {
            if(comp(SA[i-1], SA[i], t))
                newGroup[SA[i]] = newGroup[SA[i-1]] + 1;
            else
                newGroup[SA[i]] = newGroup[SA[i-1]];
        }
        for(int i = 0; i < len; i++)
            group[i] = newGroup[i];
        t <<= 1;
    }
}
//kasai's algorithm
void makeLcpArray(int len) {
	int d = 0;
	int saIndices[MAX_S];

	for(int i = 0; i < len; i++)
		saIndices[SA[i]] = i;

	LCP[0] = 0;
	for(int i = 0; i < len; i++, d--) {
		int j;
		if(d < 0) d = 0;
		if (saIndices[i] == 0) continue;

		j = SA[saIndices[i] - 1];
		while(i + d < len && j + d < len && str[i+d] == str[j + d])
			d++;
		LCP[saIndices[i]] = d;
	}
	return;
}

int run(void) {
    int len; int res = 0;
	int stack[MAX_S+1][2]; int sp = 0;
    scanf(" %d", &K);
    scanf(" %s", str);
    len = strLen(str);
    if (K == 1) return len;
    makeSuffixArray(len);
	makeLcpArray(len);

	stack[0][0] = stack[0][1] = 0;
    for (int i = 1; i < len; i++) {
		while(sp > 0 && stack[sp][0] > LCP[i]) {
			sp--;
			stack[sp][1] += stack[sp+1][1] - 1;
        	if (stack[sp][1] >= K && stack[sp][0] > res) res = stack[sp][0];
		}

    	if (stack[sp][0] < LCP[i]) {
            stack[++sp][0] = LCP[i];
            stack[sp][1] = 2;
        } else if (stack[sp][0] == LCP[i])
        	stack[sp][1]++;
		if (stack[sp][1] >= K && stack[sp][0] > res) res = stack[sp][0];
    }
	while(--sp > 0) {
		stack[sp][1] += stack[sp+1][1] - 1;
		if(stack[sp][1] >= K && stack[sp][0] > res) {
			res = stack[sp][0];
			break;
		}
	}
    return res;
}

int main(void) {
    int C;
    scanf(" %d", &C);
    while(C--) {
        printf("%d\n", run());
    }
    return 0;
}