import boto3
import sys
from botocore.exceptions import ClientError
import logging
from environs import Env

def upload(file_name):
    try:
        response = s3.upload_file("/home/stijn/iot/" + file_name, 'camera-footage', file_name)
    except ClientError as e:
        logging.error(e)
        return False
    return True

env = Env()
env.read_env()

s3 = boto3.client(
    's3',
    aws_access_key_id=env.str('AWSAK'),
    aws_secret_access_key=env.str('AWSSK')
)

result = upload(sys.argv[1])

print("Success" if result else "Failed")