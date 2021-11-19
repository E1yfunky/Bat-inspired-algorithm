from functools import wraps
from flask import render_template, session, current_app, request


def is_group_valid():
    group_name = session.get('group_name', '')
    if group_name:
        return True
    return False


def group_validation_decorator(f):
    def wrapper(*args, **kwargs):
        if is_group_valid():
            return f(*args, **kwargs)
        return render_template('premission_denied.html')
    return wrapper


def is_group_permission_valid():
    config = current_app.config['ACCESS_CONFIG']
    group_name = session.get('group_name', 'unauthorized')

    target_app = "" if len(request.endpoint.split('.')) == 1 else request.endpoint.split('.')[1]

    if group_name in config and target_app in config[group_name]:
        return True
    return False


def group_permission_decorator(f):
    @wraps(f)
    def wrapper(*args, **kwargs):
        if is_group_permission_valid():
            return f(*args, **kwargs)
        return render_template('premission_denied.html')
    return wrapper