#!/usr/bin/env python
import os
import sys
from myalign import globalvar as gl

if __name__ == "__main__":
    os.environ.setdefault("DJANGO_SETTINGS_MODULE", "wordalignweb.settings")
    sys.path.extend(['', '/Users/wangql', '/usr/local/lib/python3.6/site-packages', '/Library/Frameworks/Python.framework/Versions/3.6/lib/python36.zip', '/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6', '/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/lib-dynload', '/Users/wangql/Library/Python/3.6/lib/python/site-packages', '/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages', '/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages/setuptools-33.1.1-py3.6.egg'])
    try:
        from django.core.management import execute_from_command_line
    except ImportError as exc:
        raise ImportError(
            "Couldn't import Django. Are you sure it's installed and "
            "available on your PYTHONPATH environment variable? Did you "
            "forget to activate a virtual environment?"
        )

    gl._init()
    execute_from_command_line(sys.argv)
