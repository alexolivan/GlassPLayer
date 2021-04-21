#
# Regular cron jobs for the glassplayer package
#
0 4	* * *	root	[ -x /usr/bin/glassplayer_maintenance ] && /usr/bin/glassplayer_maintenance
