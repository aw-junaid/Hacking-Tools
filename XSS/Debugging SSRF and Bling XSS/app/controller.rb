require 'json'

class Controller
  attr_reader :req, :res, :logger

  def initialize(req, res, logger)
    @req = req
    @res = res
    @logger = logger
  end

  def params
    req.query
  end

  def get
    # 501 Not Implemented
    res.status = 501
  end

  def post
    # 501 Not Implemented
    res.status = 501
  end

  def log_request(message, extra = {})
    log = extra.merge({
      raw: req.raw_header,
      body: req.body,
      cookies: req.cookies,
      query: req.query,
      request_line: req.request_line,
      ip_address: req.remote_ip,
    })

    logger.info(message) { log.to_json }
  end
end
