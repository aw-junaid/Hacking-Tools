class ConsoleHelper
  def self.get_argument(key)
    return unless ARGV.index(key)

    ARGV[ARGV.index(key) + 1]
  end
end
