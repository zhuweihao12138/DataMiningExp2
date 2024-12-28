from http import HTTPStatus
from dashscope import Application


def call_agent_app():
    response = Application.call(app_id='16f3cc936a954399ae734fe4b71a717f',
                                prompt='Introduce the capital of China',
                                api_key='sk-9b70a2c344c14b8aabf1509515a31fb6',)

    if response.status_code != HTTPStatus.OK:
        print('request_id=%s, code=%s, message=%s\n' % (response.request_id, response.status_code, response.message))
    else:
        print('request_id=%s\n output=%s\n usage=%s\n' % (response.request_id, response.output, response.usage))


if __name__ == '__main__':
    call_agent_app()