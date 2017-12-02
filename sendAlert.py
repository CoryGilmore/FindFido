from twilio.rest import Client

account_sid ="ACe1b06b82ab93e7fee04f3d8a1e0a26db"
auth_token ="dbf6d0a0bbb0006a8075fe54e1f483a9"

client = Client(account_sid, auth_token)

message = client.api.account.messages.create(
        to="6197334477",
        from_="+18584616802",
        body="Your FindFido device has indicated your dog is no longer in range of your home network")

