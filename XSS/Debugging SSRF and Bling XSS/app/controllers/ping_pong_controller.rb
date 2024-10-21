require_relative '../controller'

class PingPongController < Controller
  def get
    res.content_type = params['content-type'] || 'text/plain'
    res.body = params['body']
  end
end
