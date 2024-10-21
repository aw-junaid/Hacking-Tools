require 'webrick'
require 'webrick/https'
require 'openssl'
require 'erb'
require 'logger'

require_relative 'controllers/redirect_controller'
require_relative 'controllers/ping_pong_controller'
require_relative 'controllers/collect_controller'
require_relative 'controllers/pixel_controller'

require_relative 'helpers/console_helper'
require_relative 'helpers/string_helper'

server = if ConsoleHelper.get_argument('-cert')
  cert = OpenSSL::X509::Certificate.new File.read ConsoleHelper.get_argument('-cert')
  pkey = OpenSSL::PKey::RSA.new File.read ConsoleHelper.get_argument('-cert')

  WEBrick::HTTPServer.new \
    Port: ConsoleHelper.get_argument('-p'),
    SSLEnable: true,
    SSLCertificate: cert,
    SSLPrivateKey: pkey
else
  WEBrick::HTTPServer.new \
    Port: ConsoleHelper.get_argument('-p')
end

trap 'INT' do
  server.shutdown
end

logger = Logger.new \
  File.open('logs/access_log', File::WRONLY | File::APPEND),
  128,
  1_024_000

CONFIG = JSON.parse(IO.read('config.json'))

[
  RedirectController,
  PingPongController,
  CollectController,
  PixelController,
].each do |klass|
  endpoint = format('/%s', klass.to_s.underscore.gsub(/_controller/, ''))

  server.mount_proc endpoint do |req, res|
    methods = %w(get post)

    if methods.include?(req.request_method.downcase)
      klass.new(req, res, logger).send(req.request_method.downcase)
    else
      res.status = 501
    end
  end
end

server.start
