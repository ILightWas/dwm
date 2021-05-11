static int dwmblockssig;
pid_t dwmblockspid = -1;

pid_t
getdwmblockspid()
{
	char buf[32], *str = buf, *c;
	FILE *fp;

	if (dwmblockspid > 0) {
		snprintf(buf, sizeof(buf), "/proc/%u/cmdline", dwmblockspid);
		if ((fp = fopen(buf, "r"))) {
			fgets(buf, sizeof(buf), fp);
			while ((c = strchr(str, '/')))
				str = c + 1;
			fclose(fp);
			if (!strcmp(str, STATUSBAR))
				return dwmblockspid;
		}
	}
	if (!(fp = popen("pidof -s "STATUSBAR, "r")))
		return -1;
	fgets(buf, sizeof(buf), fp);
	pclose(fp);
	return strtol(buf, NULL, 10);
}

void
sigdwmblocks(const Arg *arg)
{
	union sigval sv;

	if (!dwmblockssig)
		return;
	if ((dwmblockspid = getdwmblockspid()) <= 0)
		return;
	
	#if BAR_DWMBLOCKS_SIGUSR1_PATCH
	sv.sival_int = (dwmblockssig << 8) | arg->i;
	if (sigqueue(dwmblockspid, SIGUSR1, sv) == -1) {
		if (errno == ESRCH) {
			if (!getdwmblockspid())
				sigqueue(dwmblockspid, SIGUSR1, sv);
		}
	}
	#else
	sv.sival_int = arg->i;
	sigqueue(dwmblockspid, SIGRTMIN+dwmblockssig, sv);
	#endif // BAR_DWMBLOCKS_SIGUSR1_PATCH

}